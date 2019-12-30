/**
 * @file ModelHarmonic.cpp
 *
 * A wrapper for the FFT algorithm found in Ooura mathematical packages.
 *
 * This file is part of the ispeech library.
 * ispeech is iqiyi software, licensed under the MIT/X11 License. A copy of
 * the license is provided with the library in the LICENSE file.
 *
 * @package vfx_ispeech
 * @version 0.0.1-dev
 * @author Han Zhongtao
 * @date 2019-2019
 * @license http://www.opensource.org/licenses/mit-license.php MIT
 * @since 0.0.1
 */

#include "ModelHarmonic.h"
#include <algorithm>
#include <cmath>
#include <cstddef>

namespace vfx_ispeech
{
  /**
   * Initializes a model.
   *
   * Prepares for a DFT model.
   *
   * @param length input signal size (usually a power of 2)
   */
  ModelHarmonic::ModelHarmonic()
  {
  }

  /**
   * Destroys the model object and cleans up working area.
   */
  ModelHarmonic::~ModelHarmonic()
  {
  }

  /**
   * Returns a list ,about fundamental frequency.
   *
   * @param x input signal
   * @param fs sampling rate
   * @param w analysis window
   * @param N FFT size
   * @param t threshold in negative dB
   * @param minf0 mininum f0 frequency in Hz
   * @param maxf0 maximim f0 frequency in Hz
   * @param f0et error threshold in the f0 detection (ex:5)
   * @return f0 fundamental frequencies
   */
  SampleListType ModelHarmonic::f0Detection(SignalSource x,double fs,SignalSource w,std::size_t N,std::size_t H,double t,double minf0,double maxf0,double f0et)
  {
    // SampleListType result;
    if(minf0 < 0){
      LOG_ERR(nullptr,"ModelHarmonic::f0Detection Minumum fundamental frequency (minf0) smaller than 0");
      // return result;
    }

    if(maxf0 >= 10000){
      LOG_ERR(nullptr,"ModelHarmonic::f0Detection Maximum fundamental frequency (maxf0) bigger than 10000Hz");
      // return result;
    }

    if(H <= 0){
      LOG_ERR(nullptr,"ModelHarmonic::f0Detection Hop size (H) smaller or equal to 0");
    }

    std::size_t window_size = w.size();

    //size of positive spectrum
    //std::size_t hN =  (std::size_t)(N/2);

    //half analysis window size by rounding
    std::size_t hM1 = (std::size_t)std::floor((window_size+1)/2);
    //half analysis window size by floor
    std::size_t hM2 = (std::size_t)std::floor(window_size/2);

    //add zeros at beginning to center first window at sample 0
    SignalSource zeros(hM2);
    x.appendHead(zeros);
    //add zeros at the end to analyze last sample
    SignalSource zeros2(hM1);
    x.append(zeros2);

    std::size_t pin = hM1;
    std::size_t pend = x.size() -hM1;

    //initialize buffer for FFT
    SampleListType fftbuffer(N);

    //normalize analysis window
    w /=sum(w);

    //initialize f0 output
    SampleListType f0;
    //initialize f0 track
    double f0t = 0.0;
    //initialize f0 stable
    double f0stable = 0.0;

    ModelDFT dft;

    while(pin < pend){
      // select frame
      SignalSource x1(x.toArray()+pin-hM1,window_size);
      AnalysisType a = dft.Analysis(x1,w,N);
      SampleListType ploc = peakDetection(a.mX,t);
      // refine peak values
      PeakInterpolateType ipeaks = peakInterp(a.mX,a.pX,ploc);

      // convert locations to Herz
      SampleListType ipfreq(ipeaks.iploc.size());
      for(std::size_t i = 0 ; i < ipeaks.iploc.size(); i++){
	ipfreq[i]=fs*ipeaks.iploc[i]/float(N);
      }
      // find f0
      f0t = f0Twm(ipfreq,ipeaks.ipmag,f0et,minf0,maxf0,f0stable);

      if( ((f0stable ==0) && (f0t>0)) || (std::abs(f0stable-f0t)< f0stable/5.0) )
	{
	  f0stable = f0t;
	}
      else
	{
	  f0stable = 0.0;
	}
      f0.push_back(f0t);

      pin+=H;
    }

    return f0;
  }

  /**
   * Detection of the harmonics of a frame from a set of spectral peaks using f0.
   *
   * @param pfreq peak frequencies
   * @param pmag magnitudes
   * @param pphase phases
   * @param f0 fundamental frequencies
   * @param nH number of harmonics
   * @param hfreqp harmonic frequencies of previous frame
   * @param fs sampling rate
   * @param harmdevslope slop of change of the deviation allowed to perfect harmonic
   * @return AnalysisType mX is the mugnitude of spectrum,pX is the phase of spectrum.
   */
  HarmonicsType ModelHarmonic::harmonicDetection(SampleListType &pfreq,SampleListType &pmag,SampleListType &pphase,double f0,std::size_t nH,SampleListType &hfreqp,double fs,double harmDevSlope/* = 0.01*/)
  {
    HarmonicsType result;
    //if no f0 return no harmonics
    if( f0 <= 0 )
      {
	result.hfreq = SampleListType(nH,0.0);
	result.hmag = SampleListType(nH,0.0);
	result.hphase = SampleListType(nH,0.0);
	return result;
      }

    //initialize harmonic frequencies
    SampleListType hfreq(nH,0.0);
    SampleListType hmag(nH,-100.0);
    SampleListType hphase(nH,0.0);

    //initialize harmonic frequencies
    SampleListType hf(nH,0.0);
    for(std::size_t i = 1;i < nH+1;i++)
      {
	hf[i-1] = i*f0;
      }

    //if no incomming harmonic tracks initialize to harmonic series
    if(hfreqp.size() == 0)
      {
	hfreqp = hf;
      }

    //initialize harmonic index
    std::size_t hi = 0;

    while( (f0>0) && (hi<nH) && (hf[hi]< fs/2) )
      {
	//closest peak
	double min = std::abs(pfreq[0] - hf[hi]);
	std::size_t pei = 0;
	for(std::size_t i = 0; i < pfreq.size(); i++)
	  {
	    double t = std::abs(pfreq[i] - hf[hi]);
	    if( t < min )
	      {
		min = t;
		pei = i;
	      }
	  }

	//LOG_INF(nullptr,"ModelHarmonic::harmonicDetection closest peak at %lld",pei);

	//deviation from perfect harmonic
	double dev1 = min;
	//deviation from previous frame
	double dev2 = 0;
	if(hfreqp[hi]>0)
	  {
	    dev2 = std::abs(pfreq[pei]-hfreqp[hi]);
	  }
	else
	  {
	    dev2 = fs;
	  }

	double threshold = f0/3 + harmDevSlope*pfreq[pei];
	//accept peak if deviation is small
	if( (dev1 < threshold) || (dev2 < threshold) )
	  {
	    hfreq[hi] = pfreq[pei];
	    hmag[hi] = pmag[pei];
	    hphase[hi] = pphase[pei];
	  }

	hi+=1;
      }

    result.hfreq  = hfreq;
    result.hmag   = hmag;
    result.hphase = hphase;
    return result;
  }
  /**
   * Analysis of a sound using the sinusoidal harmonic model.
   *
   * @param x input signal
   * @param fs sampling rate
   * @param w analysis window
   * @param N FFT size(mininum 512)
   * @param t threshold in negative dB
   * @param minf0 mininum f0 frequency in Hz
   * @param maxf0 maximim f0 frequency in Hz
   * @param f0et error threshold in the f0 detection (ex:5)
   * @param harmDevSlope slope of harmonic deviation
   * @param minSineDur minimum length of harmonics
   * @return xhfreq, xhmag, xhphase: harmonic frequencies, magnitudes and phases
   */
  AnalysisType ModelHarmonic::Analysis(SignalSource x,double fs,SignalSource w,std::size_t N,std::size_t H,double t,std::size_t nH,double minf0,double maxf0,double f0et,double harmDevSlope /*=0.01*/,double minSineDur/*=0.02*/)
  {
    // clear last
    xhfreq_.clear();
    xhmag_.clear();
    xhphase_.clear();

    AnalysisType result;
    if(minSineDur < 0){
      LOG_ERR(nullptr,"ModelHarmonic::Analysis Minimum duration of sine tracks smaller than 0");
      return result;
    }

    std::size_t window_size = w.size();

    //size of positive spectrum
    //std::size_t hN =  (std::size_t)(N/2);

    //half analysis window size by rounding
    std::size_t hM1 = (std::size_t)std::floor((window_size+1)/2);
    //half analysis window size by floor
    std::size_t hM2 = (std::size_t)std::floor(window_size/2);

    //add zeros at beginning to center first window at sample 0
    SignalSource zeros(hM2);
    x.appendHead(zeros);
    //add zeros at the end to analyze last sample
    x.append(zeros);

    std::size_t pin = hM1;
    std::size_t pend = x.size() -hM1;

    //initialize buffer for FFT
    SampleListType fftbuffer(N);

    //normalize analysis window
    w /=sum(w);

    //initialize harmonic frequencies of previous frame
    SampleListType hfreqp(0);

    //initialize f0 track
    double f0t = 0.0;
    //initialize f0 stable
    double f0stable = 0.0;

    ModelDFT dft;

    while(pin <= pend){
      // select frame
      SignalSource x1(x.toArray()+pin-hM1,window_size);
      AnalysisType a = dft.Analysis(x1,w,N);
      SampleListType ploc = peakDetection(a.mX,t);
      // refine peak values
      PeakInterpolateType ipeaks = peakInterp(a.mX,a.pX,ploc);

      // convert locations to Herz
      SampleListType ipfreq(ipeaks.iploc.size());
      for(std::size_t i = 0 ; i < ipeaks.iploc.size(); i++){
	ipfreq[i]=fs*ipeaks.iploc[i]/float(N);
      }
      // find f0
      f0t = f0Twm(ipfreq,ipeaks.ipmag,f0et,minf0,maxf0,f0stable);

      if( ((f0stable ==0) && (f0t>0)) || ( (f0stable>0) && (std::abs(f0stable-f0t)< f0stable/5.0) ) )
	{
	  f0stable = f0t;
	}
      else
	{
	  f0stable = 0.0;
	}
      HarmonicsType harmonicsType = harmonicDetection(ipfreq,ipeaks.ipmag,ipeaks.ipphase,f0t,nH,hfreqp,fs,harmDevSlope);

      hfreqp = harmonicsType.hfreq;

      xhfreq_.push_back(harmonicsType.hfreq);
      xhmag_.push_back(harmonicsType.hmag);
      xhphase_.push_back(harmonicsType.hphase);

      pin+=H;
    }

    //delete tracks shorter than minSineDur
    cleaningSineTracks(xhfreq_,std::round(fs*minSineDur/H));

    return result;
  }

  std::size_t ModelHarmonic::GetAnalysisTypeCount()
  {
    return xhfreq_.size();
  }

  HarmonicsType ModelHarmonic::GetAnalysisTypeByIndex(std::size_t index)
  {
    HarmonicsType  harmanocsType;
    harmanocsType.hfreq = xhfreq_[index];
    harmanocsType.hmag = xhmag_[index];
    harmanocsType.hphase = xhphase_[index];

    return harmanocsType;
  }

}
