/**
 * @file functions.h
 *
 * Miscellaneous utility functions.
 *
 * This file is part of the ispeech library.
 * ispeech is free software, licensed under the MIT/X11 License. A copy of
 * the license is provided with the library in the LICENSE file.
 *
 * @package vfx_ispeech
 * @version 0.0.1-dev
 * @author Han Zhongtao
 * @date 2019-2019
 * @license
 * @since 0.0.1
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "global.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <functional>
#include "./functions/gen_spec_sines.h"
#include "./utils/log.h"
#include "./functions/dtw.h"
namespace vfx_ispeech
{
  /**
   * Converts the value to decibels (assuming reference value equal to 1).
   *
   * @param value input value
   * @return value in dB
   */
  template<typename Numeric>
  ISPEECH_EXPORT  Numeric dB(Numeric value)
  {
    return 20.0 * std::log10(value);
  }

  /**
   * Convert the magnitude of a complex number to decibels.
   *
   * @param value input value (complex number)
   * @return magnitude in dB
   */
  ISPEECH_EXPORT inline double dB(ComplexType value)
  {
    return dB(std::abs(value));
  }

  /**
   * Converts the value to decibels, relative to the reference value.
   *
   * @param value input value
   * @param refValue reference value
   * @return value in dB, relative to reference value
   */
  template<typename Numeric>
  ISPEECH_EXPORT inline Numeric dB(Numeric value, Numeric refValue)
  {
    return 20.0 * std::log10(value / refValue);
  }

  /**
   * Clamps (limits) the value inside a range.
   *
   * @param min lower limit
   * @param value numver to clamp
   * @param max upper limit
   * @return bounded value
   */
  template<typename Numeric>
  ISPEECH_EXPORT inline Numeric clamp(Numeric min, Numeric value, Numeric max)
  {
    return std::max(min, std::min(value, max));
  }

  /**
   * Returns a pseudorandom value from a range.
   *
   * @param from lower limit
   * @param to upper limit
   * @return random number
   */
  ISPEECH_EXPORT inline int random(int from, int to)
  {
    return std::rand() % (to - from) + from;
  }

  /**
   * Returns a pseudorandom double number from 0 to 1.
   */
  ISPEECH_EXPORT inline double randomDouble()
  {
    return std::rand() / static_cast<double>(RAND_MAX);
  }

  /**
   * Checks if n is an exact power of 2.
   */
  template<typename Integer>
  ISPEECH_EXPORT inline bool isPowerOf2(Integer n)
  {
    return (n > 0) && ((n & (n - 1)) == 0);
  }

  /**
   * Returns the smallest power of 2 greater than n.
   */
  template<typename Integer>
  ISPEECH_EXPORT inline Integer nextPowerOf2(Integer n)
  {
    if (isPowerOf2(n))
      {
	return 2 * n;
      }

#ifdef _MSC_VER
    size_t size_in_bits = sizeof(Integer) * 8;
#else
    constexpr size_t size_in_bits = sizeof(Integer) * 8;
#endif

    for (size_t shift = 1; shift < size_in_bits; shift *= 2)
      {
	n |= (n >> shift);
      }
    return (n + 1);
  }

  /**
   * Prototype of distance calculating functions.
   */
  typedef std::function<double(const std::vector<double>&,
			       const std::vector<double>&)> DistanceFunctionType;

  /**
   * Returns Euclidean distance between two vectors.
   *
   * @param v1 first vector
   * @param v2 second vector
   * @return Euclidean distance
   */
  ISPEECH_EXPORT inline double euclideanDistance(const std::vector<double>& v1,
						 const std::vector<double>& v2)
  {
    double distance = 0.0;
    for (std::size_t i = 0, size = v1.size(); i < size; i++)
      {
	distance += (v1[i] - v2[i])*(v1[i] - v2[i]);
      }

    return std::sqrt(distance);
  }

  /**
   * Returns Manhattan (taxicab) distance between two vectors.
   *
   * @param v1 first vector
   * @param v2 second vector
   * @return Manhattan distance
   */
  ISPEECH_EXPORT inline double manhattanDistance(const std::vector<double>& v1,
						 const std::vector<double>& v2)
  {
    double distance = 0.0;
    for (std::size_t i = 0, size = v1.size(); i < size; i++)
      {
	distance += std::abs(v1[i] - v2[i]);
      }

    return distance;
  }

  /**
   * Returns Chebyshev distance between two vectors.
   *
   * @param v1 first vector
   * @param v2 second vector
   * @return Chebyshev distance
   */
  ISPEECH_EXPORT inline double chebyshevDistance(const std::vector<double>& v1,
						 const std::vector<double>& v2)
  {
    double distance = 0.0, max = 0.0;
    for (std::size_t i = 0, size = v1.size(); i < size; i++)
      {
	distance = std::abs(v1[i] - v2[i]);
	if (distance > max)
	  {
	    max = distance;
	  }
      }

    return max;
  }

  ISPEECH_EXPORT inline void zeroSpectrum(SpectrumType &s)
  {
    std::size_t size = s.size();

    for(std::size_t i = 0; i < size; i++){
      s[i] = 0;
    }
  }

  ISPEECH_EXPORT inline void zeroSamples(SampleListType &s,double value = 0)
  {
    std::size_t size = s.size();

    for(std::size_t i = 0; i < size; i++){
      s[i] = value;
    }
  }

  /** Unwrap the signal.
   * @see https://ww2.mathworks.cn/help/dsp/ref/unwrap.html#f5-1119858

   * Definition of Phase Unwrap:
   Algorithms that compute the phase of a signal often only output phases between –π and π. For instance, such algorithms compute the phase of sin(2π + 3) to be 3, since sin(3) = sin(2π + 3), and since the actual phase, 2π + 3, is not between –π and π. Such algorithms compute the phases of sin(–4π + 3) and sin(16π + 3) to be 3 as well.
   Phase unwrap or unwrap is a process often used to reconstruct a signal's original phase. Unwrap algorithms add appropriate multiples of 2π to each phase input to restore original phase values
   * @param list wrapping signal list
   * @param discount the tolerance parameter,wich a phase jump occurs when the difference between two adjacent phase value entries exceeds the value of
   * @return list of unwrap signal
   */
  ISPEECH_EXPORT inline void unwrap(SampleListType & list,double discont = M_PI)
  {
    std::size_t size = list.size();
    long k = 0;
    std::size_t i = 1;
    SampleListType unwrap(size);
    for(; i < size-1; i++){
      unwrap[i]=list[i]+2*M_PI*k;
      //如果相邻两个相位差值大于discont,则可判定发生了相位反转
      if( std::abs(list[i]-list[i+1]) > discont )
	{
	  //从（0，pi）变化到(-pi,0),要将phase加上2pi，否则减去2pi.
	  if(list[i+1] < list[i])
	    k+=1;
	  else
	    k-=1;
	}
    }
    //处理最后一个相位
    unwrap[i] = list[i]+2*M_PI*k;

    std::transform(
		   std::begin(unwrap),
		   std::end(unwrap),
		   std::begin(list),
		   [](SampleType y) { return y; }
		   );
  }

  /**
   * Returns peak locations.
   *
   * @param mX magnitude spectrum
   * @param t threshold in nagetive dB
   * @return Chebyshev distance
   */
  ISPEECH_EXPORT inline SampleListType peakDetection(SampleListType & mX,double t)
  {
    std::size_t size = mX.size();
    SampleListType ploc;
    for(std::size_t i = 1; i < size-1; i++){

      if(mX[i] < t) continue;

      if( (mX[i] > mX[i-1]) && (mX[i] > mX[i+1]) ){
	ploc.push_back(i);
      }
    }

    return ploc;
  }

  /**
   * Returns interpolated peak location,magnitude and phase values
   * @see https://docs.scipy.org/doc/numpy/reference/generated/numpy.interp.html
   * @param x The x-coordinates at which to evaluate the interpolated values
   * @param xp 1-D sequence of floats
   * @param fp 1-D sequence of floats,the y-coordinates of the data points, same length as xp
   * @return Returns the one-dimensional piecewise linear interpolant to a function with given discrete data points (xp, fp), evaluated at x
   */
  ISPEECH_EXPORT inline SampleListType Interp1D(SampleListType &x,SampleListType &xp,SampleListType &fp)
  {
    std::size_t x_size = x.size();
    std::size_t xp_fp_size = xp.size();

    SampleListType ix(x_size,0);

    for(std::size_t i = 0; i < x_size; i++){
      if(x[i] <= xp[0]){
	ix[i]=fp[0];
	continue;
      }
      if(x[i]>=xp[xp_fp_size-1]){
	ix[i]= fp[xp_fp_size-1];
	continue;
      }
      for(std::size_t j = 0; j < xp_fp_size-1;j++){
	if( (x[i] > xp[j]) && (x[i] < xp[j+1]) ){
	  ix[i] = fp[j] + (x[i] - xp[j])/(xp[j+1] - xp[j])*(fp[j+1] - fp[j]);
	  continue;
	}
	if(x[i]==xp[j]){
	  ix[i] = fp[j];
	  continue;
	}
      }
    }

    return ix;
  }

  /**
   * Return evenly spaced values within a given interval
   * @param start Start of interval. The interval includes this value. The default start value is 0
   * @param stop End of interval. The interval does not include this value
   * @param step Spacing between values. For any output out, this is the distance between two adjacent values, out[i+1] - out[i]
   * @return Returns Array of evenly spaced values
   */
  ISPEECH_EXPORT inline SampleListType arange(SampleType start = 0.0,SampleType stop = std::numeric_limits<double>::max(),SampleType step = 1.0)
  {
    if(step == 0.0) return SampleListType(0);

    if( (stop == std::numeric_limits<double>::max() ) && (start > 0) ){
      stop = start;
      start = 0.0;
    }

    std::size_t size = std::ceil((stop-start)/step);

    SampleListType a(size);
    for(std::size_t i = 0; i < size; i++){
      a[i] = start + i*step;
    }
    return a;
  }

  /**
   * Interpolate peak values using parabolic interpolation
   * @param mX magnitude spectrum
   * @param pX phase spectrum
   * @param ploc locations of peaks
   * @return Returns PeakInterpolateType,wich is interpolated peak location, magnitude and phase values
   */
  ISPEECH_EXPORT inline PeakInterpolateType peakInterp(SampleListType &mX,SampleListType &pX,SampleListType &ploc)
  {
    std::size_t size = ploc.size();

    SampleListType iploc(size);
    SampleListType ipmag(size);
    SampleListType ipphase(size);
    for(std::size_t i = 0; i<size; i++){
      SampleType val = mX[ploc[i]];
      SampleType lval= mX[ploc[i]-1];
      SampleType rval= mX[ploc[i]+1];
      iploc[i] = ploc[i]+ 0.5*(lval-rval)/(lval-2*val+rval);
      ipmag[i]= val - 0.25*(lval-rval)*(ploc[i]-ploc[i]);
    }

    SampleListType xp = arange(0,pX.size());
    ipphase = Interp1D(iploc,xp,pX);

    PeakInterpolateType r;
    r.iploc = iploc;
    r.ipmag = ipmag;
    r.ipphase = ipphase;

    return r;
  }

  /**
   * Fast generate a spectrum from a series of sine values
   * @param ipfreq  sine peaks frequencies
   * @param ipmag magnitudes of locations of sine peaks
   * @param ipphase phases of locathions sine peaks
   * @return Returns generated complex spectrum of sines
   */
  ISPEECH_EXPORT inline SpectrumType genSpecSines(SampleListType &ipfreq,SampleListType &ipmag,SampleListType &ipphase, std::size_t N, std::size_t fs)
  {
    int peaks = (int)ipfreq.size();
    SampleListType real(N,0);
    SampleListType imag(N,0);
    SpectrumType spectrum(N);

    std::transform(
		   std::begin(ipfreq),
		   std::end(ipfreq),
		   std::begin(ipfreq),
		   [N,fs](SampleType f) { return N*f/float(fs); }
		   );


    genspecsines(ipfreq,ipmag,ipphase,peaks,real,imag,N);

    std::size_t ysize = real.size();
    //LOG_ERR(nullptr,"Function genSpecSines ysize = %lld",ysize);

    SampleListType out(ysize);
    for(std::size_t i = 0; i < ysize;i++)
      {
	out[i]= std::abs(real[i]);
	//LOG_ERR(nullptr,"Function genSpecSines out[%lld] = %f",i,out[i]);
      }

    std::transform(
		   std::begin(real),
		   std::end(real),
		   std::begin(imag),
		   std::begin(spectrum),
		   [](SampleType r,SampleType i) { return ComplexType(r,i); }
		   );

    return spectrum;
  }
  /**
   * Return the indices of the elements that are non-zero.
   * @param a input list
   * @return Returns a tuple of arrays, one for each dimension of a, containing the indices of the non-zero elements, other for no-zero elements
   */
  ISPEECH_EXPORT inline TupleList nonzero(SampleListType &a)
  {
    TupleList tup;
    std::size_t size = a.size();
    for(std::size_t i = 0; i < size; i++){
      if(a[i] != 0){
	tup.indices.push_back(i);
	tup.elements.push_back(a[i]);
      }
    }

    return tup;
  }

  /**
   * Tracking sinusoids from one frame to the next
   * @param pfreq frequencies current frame
   * @param pmag  magnitude of current frame
   * @param pphase phases of current frame
   * @param tfreq frequencies of incoming tracks from previous frame
   * @param freqDevOffset  minimum frequency deviation to be tracked
   * @param freqDevSlope slope increase of minimum frequency deviation,it take tracking is getting easylly each higher frequency.
   * @return Returns tfreqn, tmagn, tphasen: frequency, magnitude and phase of tracks
   */
  ISPEECH_EXPORT inline SineTrackingType  sineTracking(SampleListType &pfreq, SampleListType &pmag, SampleListType &pphase, SampleListType &tfreq,double freqDevOffset = 20.0,double freqDevSlope=0.01)
  {
    /*
      std::size_t pfreq_size = pfreq.size();
      LOG_ERR(nullptr,"Functions sineTracking pfreq size = %lld",pfreq_size);
      for(std::size_t i = 0; i < pfreq_size;i++) {
      LOG_ERR(nullptr,"Functions sineTracking pfreq[%lld] = %f",i,pfreq[i]);
      }
    */

    /*
      std::size_t tfreq_size = tfreq.size();
      LOG_ERR(nullptr,"Functions sineTracking input tfreq size = %lld",tfreq_size);
      for(std::size_t i = 0; i < tfreq_size;i++) {
      LOG_ERR(nullptr,"Functions sineTracking input tfreq[%lld] = %f",i,tfreq[i]);
      }
    */

    //initialize array for output frequencies
    SampleListType tfreqn(tfreq.size(),0);
    //initialize array for output magnitudes
    SampleListType tmagn(tfreq.size(),0);
    //initialize array for output phases
    SampleListType tphasen(tfreq.size(),0);

    /*
     *找到pfreq中非0项的索引，存放在pindexes中
     */
    SampleListType pindexes = nonzero(pfreq).indices;

    //找到tfreq中非0项的索引存放在incomingTracks中,incomingTracks为上一帧跟踪上的频率
    SampleListType incomingTracks = nonzero(tfreq).indices;

    SampleListType newTracks(tfreq.size(),-1);

    /*
     *生成magOrder，magOrder存储的为输入数据的索引
     */
    //按pmag中的值的升序，对输入的非0频率进行排序，并将排序后的在pfreq中的索引值存储在magOrder中
    //magOrder = np.argsort(-pmag[pindexes])
    std::vector<std::size_t> magOrder( pindexes.size() );

    struct sortitem{
      sortitem(double v,std::size_t i){value = v;index = i;}
      double value;std::size_t index;};
    std::vector<sortitem> vinindexes;
    for(std::size_t i = 0; i < pindexes.size();i++){
      vinindexes.push_back( sortitem( pmag[pindexes[i]],pindexes[i] ) );
    }
    std::sort( vinindexes.begin(),vinindexes.end(),[](sortitem a,sortitem b) { return a.value > b.value;});
    std::transform(std::begin(vinindexes),std::end(vinindexes),std::begin(magOrder),[]( sortitem x) { return x.index; });

    std::size_t magOrder_size = magOrder.size();
    //LOG_ERR(nullptr,"Functions sineTracking  magOrder_size = %lld",magOrder_size);

    /*
      for(std::size_t i = 0; i < magOrder_size;i++)
      {
      LOG_ERR(nullptr,"Functions sineTracking magOrder[%lld] = %lld",i,magOrder[i]);
      }
    */

    SampleListType pfreqt = SampleListType(pfreq.begin(),pfreq.end());
    SampleListType pmagt = SampleListType(pmag.begin(),pmag.end());
    SampleListType pphaset= SampleListType(pphase.begin(),pphase.end());
    //LOG_ERR(nullptr,"Functions sineTracking incomingTracks size =  %lld",incomingTracks.size());

    /*
     *找到所有跟踪成功的数据项
     */
    if(incomingTracks.size() > 0){
      for(std::size_t j = 0; j < magOrder_size; j++){

	if(incomingTracks.size() == 0)
	  break;

	//here i is the index of input fequencies
	std::size_t i = magOrder[j];

	//
	//找出和当前输入频率i最接近的跟踪频率索引track，然后根据指定的判断条件，判断是否为跟踪上的频率
	//

	//track = np.argmin(abs(pfreqt[i] - tfreq[incomingTracks])
	std::size_t track = 0;
	std::vector<sortitem> freqdiffs;
	for(std::size_t k = 0; k < incomingTracks.size();k++){
	  freqdiffs.push_back( sortitem( std::abs(pfreqt[i]-tfreq[incomingTracks[k]]),k ) );
	}
	std::sort( freqdiffs.begin(),freqdiffs.end(),[](sortitem a,sortitem b) { return a.value < b.value;});
	track = freqdiffs[0].index;

	//LOG_ERR(nullptr,"Functions  when magOrder size = %lld track = %lld",magOrder.size(),track);
	double freqDistance = std::abs(pfreq[i] - tfreq[incomingTracks[track]]);

	if(freqDistance < (freqDevOffset + freqDevSlope*pfreq[i])){
	  newTracks[incomingTracks[track]] = i;
	  incomingTracks.erase(incomingTracks.begin() + track);
	}
	//LOG_ERR(nullptr,"Functions  newTracks size  = %lld",newTracks.size());
      }
    }

    /*
     *对跟踪成功的所有项，根据对应的输入数据对输出数据进行赋值。
     */
    SampleListType indext;
    SampleListType indexp;
    //LOG_ERR(nullptr,"Functions sineTracking  newTracks in not -1 list:");
    for(std::size_t k = 0; k < newTracks.size(); k++){
      if(newTracks[k] != -1){
	indext.push_back(k);
	indexp.push_back(newTracks[k]);
      }

    }
    //对已经跟踪上的项进行赋值
    for(std::size_t k = 0; k < indext.size(); k++)
      {
	std::size_t index = indexp[k];
	tfreqn[indext[k]] =pfreqt[index];
	tmagn[indext[k]]  =pmagt[index];
	tphasen[indext[k]]=pphaset[index];
      }

    //从后向前进行删除已经跟踪上的数据
    std::sort( indexp.begin(),indexp.end(),[](double a,double b) { return a > b;});
    for(std::size_t k = 0; k < indexp.size(); k++){
      std::size_t index = indexp[k];
      pfreqt.erase(pfreqt.begin() + index);
      pmagt.erase(pmagt.begin() + index);
      pphaset.erase(pphaset.begin() + index);
    }

    /*
     *对没有跟踪上的tfreq中的轨道，按输入频率的幅值由到大到小的顺序进行对应填充
     *这里认为频谱中幅值较大的项比较重要，所以要优先赋值，避免后面在其他环节被忽略掉。
     */
    //获得跟踪频率为空值0的索引emptyt
    SampleListType emptyt;
    for(std::size_t k = 0; k < tfreq.size(); k++){
      if(tfreq[k] == 0)
	emptyt.push_back(k);
    }

    //将没有跟踪上的数据，按pmagt的值，从大到小排序，并把排序后的序号存放在peaksleft中
    //peaksleft = np.argsort(-pmagt)
    SampleListType peaksleft( pmagt.size(), 0);

    std::vector<sortitem> spmagt;
    for(std::size_t k = 0; k < pmagt.size();k++){
      spmagt.push_back( sortitem( pmagt[k],k ) );
    }
    std::sort( spmagt.begin(),spmagt.end(),[](sortitem a,sortitem b) { return a.value > b.value;});
    std::transform(std::begin(spmagt),std::end(spmagt),std::begin(peaksleft),[]( sortitem x) { return x.index; });

    //对剩余输出数据进行赋值处理
    if( (peaksleft.size()>0) && (emptyt.size() >= peaksleft.size()) )
      {
	for(std::size_t k = 0 ; k < peaksleft.size(); k++)
	  {
	    tfreqn[emptyt[k]] = pfreqt[peaksleft[k]];
	    tmagn[emptyt[k]]  = pmagt[peaksleft[k]];
	    tphasen[emptyt[k]]= pphaset[peaksleft[k]];
	  }
      }
    else if( (peaksleft.size() > 0) && (emptyt.size() < peaksleft.size()) )
      {
	for(std::size_t k = 0 ; k < emptyt.size(); k++ )
	  {
	    tfreqn[emptyt[k]] = pfreqt[peaksleft[k]];
	    tmagn[emptyt[k]] = pmagt[peaksleft[k]];
	    tphasen[emptyt[k]] = pphaset[peaksleft[k]];
	  }

	for(std::size_t k = emptyt.size(); k < peaksleft.size(); k++)
	  {
	    //LOG_ERR(nullptr,"Functions sineTracking  finally tfreqn[%lld] = %f",k,pfreqt[peaksleft[k]]);

	    tfreqn.push_back( pfreqt[peaksleft[k]] );
	    tmagn.push_back( pmagt[peaksleft[k]] );
	    tphasen.push_back( pphaset[peaksleft[k]] );
	  }
      }

    /*
      std::size_t ysize = tfreqn.size();
      LOG_ERR(nullptr,"Functions sineTracking ysize = %lld",ysize);
      for(std::size_t k = 0; k < ysize;k++) {
      LOG_ERR(nullptr,"Functions sineTracking finally tfreqn[%lld] = %f",k,tfreqn[k]);
      }
    */

    SineTrackingType tracking;
    tracking.tfreqn = tfreqn;
    tracking.tmagn = tmagn;
    tracking.tphasen = tphasen;

    return tracking;
  }
  /**
   * Delete short fragments of a collection of sinusoidal tracks
   *
   * @param xtfreq frequency of tracks
   * @param minTrackLength minimum duration of tracks in number of frames
   * @return output frequency of tracks
   */
  ISPEECH_EXPORT inline void cleaningSineTracks(std::vector<SampleListType> &xtfreq,std::size_t minTrackLength)
  {
    std::size_t nFrames = xtfreq.size();
    if( nFrames == 0 ) return;
    std::size_t nTracks = xtfreq[0].size();

    for(std::size_t track = 0; track < nTracks; track++)
      {
	bool b_none_zero = false;
	std::size_t countors = 0;
	std::size_t begining = 0;
	std::size_t end;
	for(std::size_t frame_index = 0; frame_index < nFrames;frame_index++)
	  {
	    if(xtfreq[frame_index][track] > 0)
	      {
		if(!b_none_zero)
		  {
		    begining = frame_index;
		    b_none_zero = true;
		  }
		countors++;
	      }
	    else
	      {
		if(b_none_zero)
		  {
		    end = frame_index;
		    if(countors <= minTrackLength)
		      {
			for(std::size_t k = begining; k < end; k++)
			  {
			    xtfreq[k][track] = 0;
			  }
		      }
		  }
		b_none_zero = false;
		begining = 0;
		countors = 0;
	      }

	    if(frame_index == (nFrames-1))
	      {
		if(b_none_zero)
		  {
		    end = frame_index;
		    if(countors <= minTrackLength)
		      {
			for(std::size_t k = begining; k <= end; k++)
			  {
			    xtfreq[k][track] = 0;
			  }
		      }
		  }
	      }
	  }//end for frame_index
      }//end for t

  }
  /**
   *  Function that wraps the f0 detection function TWM, selecting the possible f0 candidates
   * @param pfreq  sine peaks frequencies in Hz
   * @param pmag magnitudes of locations of sine peaks
   * @param f0c frequencies of f0 candidates
   * @param out_f0 fundamental frequency detected
   * @param out_f0error Twm error of fundamental frequency

   * @return Return void
   */
  ISPEECH_EXPORT inline void Twm(SampleListType &pfreq,SampleListType &pmag,SampleListType f0c,double &out_f0,double &out_f0error)
  {
    std::size_t maxnpeaks = 10;

    //some of empirically values
    double p = 0.5;// weighting by frequency value
    double q = 1.4;// weighting related to magnitude of peaks
    double r = 0.5; // scaling related to magnitude of peaks
    double rho = 0.33;// weighting of MP error

    auto it = std::max_element(pmag.begin(),pmag.end());
    double Amax = *it;
    SampleListType frequencies = pfreq;
    SampleListType mags = pmag;

    // calculate predicted to measured error
    std::size_t MaxNPM = std::min(maxnpeaks,pfreq.size());

    SampleListType error_p_to_m(f0c.size(),0);
    for(std::size_t f = 0; f < f0c.size(); f++ )
      {
	double ffund = f0c[f];
	SampleListType fn(MaxNPM,0);
	SampleListType deltafn(MaxNPM,0);
	//make all of fn
	for(std::size_t n = 0; n < MaxNPM; n++)
	  {
	    fn[n] = ffund + n*ffund;
	    deltafn[n] = std::abs(fn[n]-frequencies[0]);

	    double an = mags[0];
	    for(std::size_t i = 0; i < frequencies.size(); i++)
	      {
		double absfk = std::abs(fn[n]- frequencies[i]);
		if(absfk < deltafn[n])
		  {
		    deltafn[n] = absfk;
		    an = mags[i];
		  }
	      }
	    double pow_part = deltafn[n]*std::pow(fn[n],-p);
	    error_p_to_m[f]+=pow_part + std::pow(10.0,(an-Amax)/20)*(q*pow_part - r);
	  }
      }

    //calculate measured to predicted error.
    std::size_t MaxNMP = std::min(maxnpeaks,pfreq.size());
    SampleListType error_m_to_p(f0c.size(),0);

    for(std::size_t f = 0; f < f0c.size(); f++)
      {
	double ffund = f0c[f];
	std::size_t maxN = std::ceil(frequencies[MaxNMP-1]/ffund);
	SampleListType fn(maxN,0);
	for(std::size_t n = 0; n < maxN;n++)
	  {
	    fn[n] = ffund + n*ffund;
	  }
	SampleListType deltafk(MaxNMP,0);
	SampleListType fk(MaxNMP,0);

	for(std::size_t k = 0; k < MaxNMP; k++)
	  {
	    fk[k] = frequencies[k];
	    deltafk[k]=std::abs(fn[0]-fk[k]);
	    double ak = mags[k];

	    for(std::size_t n = 0; n < maxN;n++)
	      {
		double absfk = std::abs(fn[n]-fk[k]);
		if(absfk < deltafk[k])
		  {
		    deltafk[k] = absfk;
		    ak = mags[k];
		  }
	      }
	    double MagFactor = std::pow(10.0,(ak-Amax)/20);
	    double pow_part = deltafk[k]*std::pow(fk[k],-p);
	    error_m_to_p[f]+=MagFactor*(pow_part + MagFactor*(q*pow_part -r));
	  }
      }

    SampleListType Error(f0c.size(),0);
    for(std::size_t i = 0; i < f0c.size(); i++)
      {
	Error[i] = error_p_to_m[i]/MaxNPM +(rho*error_m_to_p[i]/MaxNMP);
      }

    std::size_t f0index = 0;
    double mininum = Error[0];


    for(std::size_t i = 0; i < Error.size(); i++)
      {
	if(Error[i] < mininum)
	  {
	    mininum = Error[i];
	    f0index = i;
	  }
      }

    out_f0 = f0c[f0index];
    out_f0error = Error[f0index];

  }
  /**
   *  Function that wraps the f0 detection function TWM, selecting the possible f0 candidates,Two-way mismatch algorithm for f0 detection (by Beauchamp&Maher)
   * @param pfreq  sine peaks frequencies in Hz
   * @param pmag magnitudes of locations of sine peaks
   * @param ef0max maximum error allowed,to be f0,wich below ef0max
   * @param minf0 mininum f0
   * @param maxf0 maxinum f0
   * @param f0t f0 of previous frame if stable

   * @return Return fundamental frequency in Hz
   */
  ISPEECH_EXPORT inline double f0Twm(SampleListType &pfreq,SampleListType &pmag,double ef0max,double minf0,double maxf0,double f0t = 0)
  {
    if(minf0 < 0){
      LOG_ERR(nullptr,"functions f0Twmf: Minumum fundamental frequency (minf0) smaller than 0");
      return 0.0;
    }

    if(maxf0 >= 10000){
      LOG_ERR(nullptr,"functions f0Twmf: Maximum fundamental frequency (maxf0) bigger than 10000Hz");
      return 0.0;
    }

    // return 0 if less than 3 peaks and not previous f0
    if( (pfreq.size() < 3) && (f0t == 0) ){
      return 0.0;
    }
    //use only peaks within given range
    PositionListType f0c;

    for(Position i = 0; i < pfreq.size(); i++)
      {
	if( (pfreq[i] > minf0) && (pfreq[i] < maxf0) )
	  {
	    f0c.push_back(i);
	  }
      }

    // return 0 if no peaks within range
    if(f0c.size() == 0)
      return 0.0;

    //frequencies of peak candidates
    SampleListType f0cf;
    //magnitude of peak candidates
    SampleListType f0cm;

    for(Position i = 0; i < f0c.size(); i++)
      {
	f0cf.push_back(pfreq[f0c[i]]);
	f0cm.push_back(pmag[f0c[i]]);
      }

    //if stable f0 in previous frame
    if(f0t > 0)
      {
	//use only peaks close to it
	PositionListType shortlist;
	for(Position i = 0; i < f0cf.size(); i++)
	  {
	    if(std::abs(f0cf[i]-f0t) < f0t/2.0)
	      {
		shortlist.push_back(i);
	      }
	  }
	Position maxc = 0;
	double value = 0;
	for(Position i = 0; i < f0cm.size();i++)
	  {
	    if(i == 0)
	      {
		value = f0cm[i];
		maxc = i;
		continue;
	      }

	    if(f0cm[i] > value)
	      {
		value = f0cm[i];
		maxc = i;
	      }
	  }
	double maxcfd = ((std::size_t)f0cf[maxc])%((std::size_t)f0t);

	if(maxcfd > f0t/2)
	  {
	    maxcfd = f0t-maxcfd;
	  }
	//or the maximum magnitude peak is not a harmonic
	if(maxcfd > f0t/4)
	  {
	    bool binshortlist = false;

	    for(Position i = 0; i < shortlist.size(); i++)
	      {
		if(shortlist[i] == maxc)
		  {
		    binshortlist=true;
		    break;
		  }
	      }
	    if(!binshortlist)
	      {
		shortlist.push_back(maxc);
	      }
	  }

	//frequencies of candidates
	SampleListType f0cf_shadow = f0cf;
	f0cf.clear();
	for(int i = 0; i < shortlist.size(); i++)
	  {
	    f0cf.push_back(f0cf_shadow[i]);
	  }
      }
    //return 0 if no peak candidates
    if(f0cf.size() == 0)
      return 0.0;

    //call the TWM function with peak candidates
    double f0 = 0.0;
    double f0error = 0.0;
    Twm(pfreq,pmag,f0cf,f0,f0error);

    //LOG_INF(nullptr,"functions f0Twmf: f0,f0error =%f,%f ",f0,f0error);
    //accept and return f0 if below max error allowed
    if( (f0 > 0) && (f0error < ef0max) )
      return f0;

    return 0.0;
  }

  /**
   *  Function fast LPC by Burg
   * @param x input signal
   * @param N size of input
   * @param a lpc coefficients
   * @param M size of coefficients

   * @return Return gain
   */
ISPEECH_EXPORT inline double LPCBurg(const double x[]/*in wave data*/, size_t N, double a[]/*out LPC*/, size_t M)
{
    std::vector<double> b1 (N, 0);
    std::vector<double> b2 (N, 0);
    std::vector<double> aa (M, 0);

    // (3)

    double gain = 0.0;
    for(size_t j = 0; j < N; j++)
    {
	gain += x[j] * x[j];
    }

    gain /= N;
    if(gain <= 0)
    {
	return 0.0;    // warning empty
    }

    // (9)

    b1[0] = x[0];
    b2[N - 2] = x[N - 1];
    for(size_t j = 1; j < N - 1; j++)
    {
	b1[j] = b2[j - 1] = x[j];
    }

    for(size_t i = 0; ; ++i)
    {
	// (7)

	double num = 0.0, denum = 0.0;
	for(size_t j = 0; j < N - i - 1; j++)
	{
	    num += b1[j] * b2[j];
	    denum += b1[j] * b1[j] + b2[j] * b2[j];
	}

	if(denum <= 0)
	{
	    return 0.0;    // warning ill-conditioned
	}

	a[i] = 2.0 * num / denum;

	// (10)

	gain *= 1.0 - a[i] * a[i];

	// (5)

	for(size_t j = 0; j+1 <= i; j++)
	{
	    a[j] = aa[j] - a[i] * aa[i - j - 1];
	}

	if(i == M-1) break;

	// (8)  Watch out: i -> i+1

	for(size_t j = 0; j <= i; j++)
	{
	    aa[j] = a[j];
	}

	for(size_t j = 0; j <= N - i - 2; j++)
	{
	    b1[j] -= aa[i] * b2[j];
	    b2[j] = b2[j + 1] - aa[i] * b1[j + 1];
	}
    }

    return gain;
}

}

#endif // FUNCTIONS_H
