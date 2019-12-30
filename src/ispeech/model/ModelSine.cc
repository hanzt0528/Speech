/**
 * @file ModelSine.cpp
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

#include "ModelSine.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <random>

namespace vfx_ispeech
{
  /**
   * Initializes a model.
   *
   * Prepares for a DFT model.
   *
   * @param length input signal size (usually a power of 2)
   */
  ModelSine::ModelSine()
  {
  }

  /**
   * Destroys the model object and cleans up working area.
   */
  ModelSine::~ModelSine()
  {
  }

  /**
   * Returns a complex data spectrum value.
   *
   * @param x input signal
   * @param fs sampling rate
   * @param w analysis window
   * @param N size of complex spectrum,it means the FFT size too
   * @param t threshold in nagative dB
   * @return output array sound
   */
  SampleListType ModelSine::sineModel(SignalSource &x,double fs,SignalSource &w,std::size_t N,double t)
  {
     std::size_t window_size = w.size();

     //half analysis window size by rounding
     std::size_t hM1 = (std::size_t)std::floor((window_size+1)/2);//128

     //half analysis window size by floor
     std::size_t hM2 = (std::size_t)std::floor(window_size/2);//128

     //FFT size for synthesis(even)
     std::size_t Ns = 512;

     //Hop size used for analysis and synthesis
     std::size_t H = Ns/4;
     //half of synthesis FFT size
     std::size_t hNs = Ns/2;

     //init sound pointer in middle of anal window
     std::size_t pin = std::max(hNs,hM1);
     //last sample to start a frame
     std::size_t pend = x.size()-std::max(hNs,hM1);

     LOG_ERR(this,"ModelSine::sineModel pin = %lld,pend = %lld",pin,pend);
     //initialize buffer for FFT
     //SampleListType fftbuffer(N);
     //zeroSamples(fftbuffer);

     //initialize output sound frame
     SampleListType yw(Ns,0);
     //initialize output array
     SampleListType y(x.size(),0);

     //normalize analysis window
     w/=sum(w);

     //initialize analysis window
     SampleListType sw(Ns,0);

     //triangular window
     TriangWindow ow(2*H);

     //add triangular window
     for(std::size_t i = hNs-H,j=0;i<hNs+H;i++,j++){
       sw[i]=ow.sample(j);
     }

     //blackmanharris window
     BlackmanharrisWindow bh(Ns);

     //normalize blackmanharris window
     bh/=sum(bh);

     //normalized synthesis window
     for(std::size_t i = 0;i<Ns;i++){
       sw[i]/=bh.sample(i);
     }

     ModelDFT dft;
     OouraFft fft(Ns);

     while(pin < pend){
       //analysis
       SignalSource x1(x.toArray()+pin-hM1,window_size);
       AnalysisType a = dft.Analysis(x1,w,N);
       SampleListType ploc = peakDetection(a.mX,t);

       PeakInterpolateType ipeaks = peakInterp(a.mX,a.pX,ploc);

       /*
	 std::size_t ysize = ipeaks.iploc.size();
	 LOG_ERR(this,"ModelSine::sineModel ysize = %lld",ysize);

	 SampleListType out(ysize);
	 for(std::size_t i = 0; i < ysize;i++)
	 {
	 out[i]= ipeaks.iploc[i];
	 LOG_ERR(this,"ModelSine::sineModel out[%lld] = %f",i,out[i]);

	 }

	 return out;
       */

       SampleListType ipfreq(ipeaks.iploc.size());

       for(std::size_t i = 0 ; i < ipeaks.iploc.size(); i++){
	 ipfreq[i]=fs*ipeaks.iploc[i]/float(N);
       }
       //synthesis
       SpectrumType Y = genSpecSines(ipfreq,ipeaks.ipmag,ipeaks.ipphase,Ns,fs);

       /*
	 std::size_t ysize = Y.size();
	 LOG_ERR(this,"ModelSine::sineModel ysize = %lld",ysize);

	 SampleListType out(ysize);
	 for(std::size_t i = 0; i < ysize;i++)
	 {
	 out[i]= Y[i].real();
	 LOG_ERR(this,"ModelSine::sineModel out[%lld] = %e",i,out[i]);

	 }
       */

       SampleListType fftbuffer = fft.ifft2(Y);
       /*
	 std::size_t ysize = fftbuffer.size();
	 LOG_ERR(this,"ModelSine::sineModel ysize = %lld",ysize);

	 SampleListType out(ysize);
	 for(std::size_t i = 0; i < ysize;i++)
	 {
	 out[i]= fftbuffer[i];
	 LOG_ERR(this,"ModelSine::sineModel out[%lld] = %f",i,out[i]);

	 }
	 return out;
       */
       for(std::size_t i = hNs+1,fi=0;fi < hNs-1; i++,fi++)
	 {
	   yw[fi] = fftbuffer[i];
	 }

       for(std::size_t i = 0,fi=hNs-1;i < hNs; i++,fi++)
	 {
	   yw[fi] = fftbuffer[i];
	 }

       for(std::size_t i = pin-hNs,k=0;i<pin+hNs; i++,k++)
	 {
	   y[i]+=sw[k]*yw[k];
	 }

       pin+=H;
     }

    return y;
  }

  /**
   * Returns the mugnitude and phase of signal at frequency-domain.
   *
   * @param x input signal
   * @param w analysis window
   * @param N FFT size
   * @return AnalysisType mX is the mugnitude of spectrum,pX is the phase of spectrum.
   */
  AnalysisType ModelSine::Analysis(SignalSource &x,double fs,SignalSource &w,std::size_t N,std::size_t H,double t,std::size_t maxnSines /*= 100*/,double minSineDur/*= 0.01*/,double freqDevOffset /*= 20.0*/,double freqDevSlope/*=0.01*/)
  {
    xtfreq.clear();
    xtmag.clear();
    xtphase.clear();

    if(minSineDur < 0){
      AnalysisType e;
      return e;
    }

     std::size_t window_size = w.size();

     if(window_size > N){
       LOG_ERR(this,"ModelSine::Analysis Window size (M=%lld) is bigger than FFT size (N=%lld)!",window_size,N);

       AnalysisType e;
       return e;
     }

    //if all x == 0 return 0.

     std::size_t hN = N/2 + 1;//257
     std::size_t hM1 = (window_size + 1) / 2;//128
     std::size_t hM2 = (std::size_t)std::floor(window_size/2);//128

     SignalSource zeros(hM2);
     // add zeros at beginning to center first window at sample 0
     x.appendHead(zeros);
     // add zeros at the end to analyze last sample
     x.append(zeros);

     //init sound pointer in middle of anal window
     std::size_t pin = hM1;
     //last sample to start a frame
     std::size_t pend = x.size()- hM1;

     //LOG_ERR(this,"ModelSine::Analysis pin = %lld,pend = %lld",pin,pend);

     //normalize analysis window
     w/=sum(w);

     SampleListType tfreq;

     ModelDFT dft;

     while( pin < pend){
       // select frame
       SignalSource x1(x.toArray()+pin-hM1,window_size);
       // compute dft
       AnalysisType a = dft.Analysis(x1,w,N);
       // detect locations of peaks
       SampleListType ploc = peakDetection(a.mX,t);
       // refine peak values by interpolation
       PeakInterpolateType ipeaks = peakInterp(a.mX,a.pX,ploc);

       // convert peak locations to Hertz
       SampleListType ipfreq(ipeaks.iploc.size());
       for(std::size_t i = 0 ; i < ipeaks.iploc.size(); i++){
	 ipfreq[i]=fs*ipeaks.iploc[i]/(float)N;
       }

       //LOG_ERR(this,"ModelSine::Analysis after peakInterp the result :ipeaks.iploc.size  = %lld",ipeaks.iploc.size());

       // perform sinusoidal tracking by adding peaks to trajectories
       SineTrackingType  tracking  = sineTracking(ipfreq, ipeaks.ipmag, ipeaks.ipphase, tfreq, freqDevOffset, freqDevSlope);


       tfreq = tracking.tfreqn;
       if(tfreq.size() > maxnSines)
	 {
	   tfreq.erase(tfreq.begin()+std::min(maxnSines,tfreq.size()),tfreq.end());
	 }
       else
	 {
	   for(std::size_t k = tfreq.size(); k < maxnSines; k++)
	     {
	       tfreq.push_back(0.0);
	     }
	 }

       std::size_t ysize = tfreq.size();
       //LOG_ERR(nullptr,"Functions sineTracking finally tracked number = %lld",ysize);
       /*
       for(std::size_t k = 0; k < ysize;k++) {
	 LOG_ERR(nullptr,"Functions sineTracking finally tfreqn[%lld] = %f",k,tfreq[k]);
       }
       */

       SampleListType tfreqn(maxnSines,0.0);
       SampleListType tmagn(maxnSines,0.0);
       SampleListType tphasen(maxnSines,0.0);

       for(std::size_t k = 0; k < maxnSines; k++)
	 {
	   if(tfreq.size() > k)
	     {
	       tfreqn[k] = tfreq[k];
	     }
	   else
	     {
	       tfreqn[k] = 0.0;
	     }

	   if(tracking.tmagn.size() > k)
	     {
	       tmagn[k] = tracking.tmagn[k];
	     }
	   else
	     {
	       tmagn[k] = 0.0;
	     }

	   if(tracking.tphasen.size() > k)
	     {
	       tphasen[k] = tracking.tphasen[k];
	     }
	   else
	     {
	       tphasen[k] = 0.0;
	     }
	 }

       xtfreq.push_back(tfreqn);
       xtmag.push_back(tmagn);
       xtphase.push_back(tphasen);

       pin+=H;
     }

     //clean sine tracks shorter than minSineDur
     cleaningSineTracks(xtfreq,fs*minSineDur/H);

    AnalysisType result;

    return result;
  }

  std::size_t ModelSine::GetAnalysisTypeCount()
  {
    return xtfreq.size();
  }

  SineTrackingType ModelSine::GetAnalysisTypeByIndex(std::size_t index)
  {
    SineTrackingType  tracking;
    tracking.tfreqn = xtfreq[index];
    tracking.tmagn = xtmag[index];
    tracking.tphasen = xtphase[index];

    return tracking;
  }

  /**
   * Returns the synthesis signal.
   *
   * @param tfreq frequencies of sinusoids
   * @param tmag magnitudes of sinusoids
   * @param tphase phases of sinusoids
   * @param N Synthesis FFT size
   * @param H hop size
   * @param fs sampling rate
   * @return synthesis of a signal using the discrete Fourier transfom
   */
  SampleListType ModelSine::Synthesis(SampleList2DType &tfreq,SampleList2DType &tmag,SampleList2DType tphase,std::size_t N,std::size_t H,double fs)
  {
    //half of FFT size for synthesis
    std::size_t hN = N/2;

    // number of frames
    std::size_t L = tfreq.size();

    //initialize output sound pointer
    std::size_t pout = 0;
    //output sound size
    std::size_t ysize = H*(L+3);
    //initialize output array
    SampleListType y(ysize,0);
    LOG_ERR(nullptr,"ModelSine::Synthesis ysize = %lld",ysize);
    //initialize synthesis window
    SampleListType sw(N,0);

    //triangular window
    TriangWindow ow(2*H);

     //put triangular window at the center of sw
     for(std::size_t i = hN-H,j=0;i<hN+H;i++,j++){
       sw[i]=ow.sample(j);
     }

     //blackmanharris window
     BlackmanharrisWindow bh(N);

     //normalize blackmanharris window
     //because of generate signal we will use the main lob of blackmanharris,so had to use hb to inverse.
     bh/=sum(bh);

     //normalized synthesis window
     for(std::size_t i = 0;i < N;i++){
       sw[i]/=bh.sample(i);
     }

     //initialize synthesis frequencies
     SampleListType lastytfreq(tfreq[0].begin(),tfreq[0].end());
     //initialize synthesis phases
    std::random_device rd;
    std::map<int, int> hist;
    std::uniform_int_distribution<int> dist(0, 99);
    SampleListType ytphase(tfreq[0].size());

    for(std::size_t k = 0; k < tfreq[0].size(); k++)
      {
	//random float value between 0.0 and 2pi
	ytphase[k] = 2*M_PI*dist(rd)/100.0;
      }

    OouraFft fft(N);
    for(std::size_t l = 0; l < L; l++)
      {
	if(tphase.size() > 0)
	  {
	    for(std::size_t k = 0; k < tphase[l].size(); k++)
	      {
		ytphase[k]=tphase[l][k];
	      }
	  }
	else
	  {
	    for(std::size_t k = 0; k < tfreq[l].size(); k++)
	      {
		ytphase[k] +=(M_PI*(lastytfreq[k]+tfreq[l][k])/fs)*H;
	      }
	  }

	SpectrumType Y = genSpecSines(tfreq[l],tmag[l],ytphase,N,fs);

	for(std::size_t k = 0; k < tfreq[l].size(); k++)
	  {
	    lastytfreq[k]=tfreq[l][k];
	  }

	for(std::size_t k = 0; k < ytphase.size(); k++)
	  {
	    ytphase[k] = fmod(ytphase[k],2*M_PI);
	  }

	//ifft
	SampleListType fftbuffer = fft.ifft2(Y);

	//fftshift
	SampleListType yw(N);
	for(std::size_t fi = 0,i = hN+1; fi < hN-1; i++,fi++ )
	  {
	    yw[fi] = fftbuffer[i];
	  }
	for(std::size_t fi=hN-1,i=0; i < hN;i++,fi++)
	  {
	    yw[fi]=fftbuffer[i];
	  }

	//
	for(std::size_t k = 0; k < N; k++)
	  {
	    y[pout+k]+=sw[k]*yw[k];
	  }
	pout+=H;
      }

    y.erase(y.begin(),y.begin()+hN);
    y.erase(y.begin()+y.size()-hN,y.end());

    SampleListType result;
    for(size_t i = hN;i<y.size()-hN;i++)
      {
	//result.push_back(y[i]);
      }
    //return result;
    return y;
  }
  SampleListType ModelSine::AnalysisAndSynthesis(SignalSource &x,double fs,SignalSource &w,std::size_t N,std::size_t H,double t,std::size_t maxnSines /*= 100*/,double minSineDur/*= 0.01*/,double freqDevOffset /*= 20.0*/,double freqDevSlope/*=0.01*/)
  {
    Analysis(x,fs,w,N,H,t,maxnSines,minSineDur,freqDevOffset,freqDevSlope);
    LOG_ERR(nullptr,"ModelSine::AnalysisAndSynthesis Analysis done");

    N=512;

    SampleListType y = Synthesis(xtfreq,xtmag,xtphase,N,H,fs);

    LOG_ERR(nullptr,"ModelSine::AnalysisAndSynthesis Synthesis done");
    SampleListType result(y.begin(),y.end());

    LOG_ERR(nullptr,"ModelSine::AnalysisAndSynthesis Synthesis result size = %lld",result.size());

    return result;
  }

}
