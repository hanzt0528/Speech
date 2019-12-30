/**
 * @file ModelDFT.cpp
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

#include "ModelDFT.h"
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
  ModelDFT::ModelDFT()
  {
  }

  /**
   * Destroys the model object and cleans up working area.
   */
  ModelDFT::~ModelDFT()
  {
  }

  /**
   * Returns a complex data spectrum value.
   *
   * @param x input signal
   * @param w analysis window
   * @param N FFT size
   * @return spectrum value at given frame and peak number
   */
  SampleListType ModelDFT::dftModel(SignalSource &x,SignalSource &w,std::size_t N)
  {
    if(!isPowerOf2(N)){
      LOG_ERR(this,"ModelDFT::dftModel FFT size (N=%lld) is not a power of 2!",N);

       SampleListType e(0);
       return e;
    }

     std::size_t window_size = w.size();

     if(window_size > N){
       LOG_ERR(this,"ModelDFT::dftModel Window size (M=%lld) is bigger than FFT size (N=%lld)!",window_size,N);

       SampleListType e(0);
       return e;
     }

    //if all x == 0 return 0.

     std::size_t hN = N/2 + 1;//257
     //std::size_t hM1 = (window_size + 1) / 2;//128
     std::size_t hM2 = (std::size_t)std::floor(window_size/2);//128

     //LOG_ERR(this,"hN=%lld,hM1= %lld,hM2=%lld",hN,hM1,hM2);

    //for sythensis
    SampleListType y(window_size);
    x*=w;

    //zero-padding
    SampleListType fftbuffer(N);
    std::transform(
		   std::begin(fftbuffer),
		   std::end(fftbuffer),
		   std::begin(fftbuffer),
		   [] (SampleType x) { return 0.0; }
		   );

    const SampleType *xw = x.toArray();

    for(std::size_t i = hM2,fi=0;i < window_size; i++,fi++)
      {
	fftbuffer[fi] = xw[i];
      }

    for(std::size_t i = 0,fi=N-hM2; i < hM2; i++,fi++)
      {
	fftbuffer[fi] = xw[i];
      }


    Dft dft(N);

    SpectrumType X = dft.fft2(fftbuffer);


    SpectrumType absX(hN);

    for(std::size_t i = 0; i < hN; i++){
      absX[i] = std::abs(X[i]);
      if(absX[i].real() < LOC)
	{
	  absX[i] = 0.0;
	  LOG_INF(this,"ModelDFT::dftModel too small");
	}
    }

    SampleListType mX(hN);
    for(std::size_t i = 0; i < hN; i++){
      mX[i] = dB(absX[i]);
    }


    SampleListType pX(hN);
    for(std::size_t i = 0; i < hN; i++){
      pX[i] = std::arg(X[i]);
    }

    SpectrumType Y(N);
    zeroSpectrum(Y);

    for(std::size_t i = 0; i < hN; i++){
      Y[i] = std::pow(10,mX[i]/20.0)*std::exp(j1*pX[i]);
    }

    for(std::size_t i = hN,k=hN-2; i < N; i++,k--){
      Y[i] = std::pow(10,mX[k]/20.0)*std::exp(-j1*pX[k]);
    }

    /*
    for(std::size_t i=0; i < window_size; i++){
      y[i]=Y[i*2].real();
    }

    return y;
    */

    fftbuffer = dft.ifft2(Y);


    for(std::size_t i = 0; i < hM2; i++){
      y[i] = fftbuffer[N - hM2+i];
    }

    for(std::size_t i=hM2,k=0; i < window_size; i++,k++){
      y[i]=fftbuffer[k];
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
  AnalysisType ModelDFT::Analysis(SignalSource &x,SignalSource &w,std::size_t N)
  {
    if(!isPowerOf2(N)){
      LOG_ERR(this,"ModelDFT::Analysis FFT size (N=%lld) is not a power of 2!",N);

      AnalysisType e;
      return e;
    }

     std::size_t window_size = w.size();

     if(window_size > N){
       LOG_ERR(this,"ModelDFT::Analysis Window size (M=%lld) is bigger than FFT size (N=%lld)!",window_size,N);

       AnalysisType e;
       return e;
     }

    //if all x == 0 return 0.

     std::size_t hN = N/2 + 1;//257
     //std::size_t hM1 = (window_size + 1) / 2;//128
     std::size_t hM2 = (std::size_t)std::floor(window_size/2);//128

     //LOG_ERR(this,"hN=%lld,hM1= %lld,hM2=%lld",hN,hM1,hM2);

    //for sythensis
    SampleListType y(window_size);

    //w/= sum(w);
    x*=w;

    //zero-padding
    SampleListType fftbuffer(N);
    std::transform(
		   std::begin(fftbuffer),
		   std::end(fftbuffer),
		   std::begin(fftbuffer),
		   [] (SampleType x) { return 0.0; }
		   );

    const SampleType *xw = x.toArray();

    for(std::size_t i = hM2,fi=0;i < window_size; i++,fi++)
      {
	fftbuffer[fi] = xw[i];
      }

    for(std::size_t i = 0,fi=N-hM2; i < hM2; i++,fi++)
      {
	fftbuffer[fi] = xw[i];
      }


    Dft dft(N);

    SpectrumType X = dft.fft2(fftbuffer);


    SpectrumType absX(hN);

    for(std::size_t i = 0; i < hN; i++){
      absX[i] = std::abs(X[i]);
      if(absX[i].real() < LOC)
	{
	  absX[i] = 0.0;
	  //LOG_INF(this,"ModelDFT::Analysis absX[i].real() too small,so set it to zero.");
	}
    }


    SampleListType mX(hN);
    for(std::size_t i = 0; i < hN; i++){
      mX[i] = dB(abs(X[i]));
    }

    /*
    for(std::size_t i = 0; i < hN; i++){
      if(X[i].real() < LOC)
	{
	  X[i].real(0.0);
	}
      if(X[i].imag() < LOC)
	{
	  X[i].imag(0.0);
	}
    }
    */
    SampleListType pX(hN);
    for(std::size_t i = 0; i < hN; i++){
      pX[i] = std::arg(X[i]);
    }

    AnalysisType result;

    unwrap(pX);
    result.mX = mX;
    result.pX = pX;
    return result;
  }
  /**
   * Returns the synthesis signal.
   *
   * @param mX magnitude spectrum
   * @param pX phase spectrum
   * @param M window size
   * @return synthesis of a signal using the discrete Fourier transfom
   */
  SampleListType ModelDFT::Synthesis(SampleListType mX,SampleListType pX,std::size_t M)
  {
    //size of positive spectrum,it includes sample 0
    std::size_t hN = mX.size();
    //FFT size
    std::size_t N = (hN-1)*2;

    if(!isPowerOf2(N)){
      LOG_ERR(this,"ModelDFT::Synthesis size of mX is not (N/2)+1!");

      SampleListType e(0);
      return e;
    }

    //std::size_t hM1 = (std::size_t)std::floor((M+1)/2);
    std::size_t hM2 = (std::size_t)std::floor(M/2);

    SampleListType fftbuffer(N,0);
    SampleListType y(M,0);

    //跟输入的正频率恢复出整个频率的频谱
    SpectrumType Y(N,0);
    for(std::size_t i = 0; i < hN; i++){
      Y[i] = std::pow(10,mX[i]/20.0)*std::exp(j1*pX[i]);
    }

    for(std::size_t i = hN,k=hN-2; i < N; i++,k--){
      Y[i] = std::pow(10,mX[k]/20.0)*std::exp(-j1*pX[k]);
    }

    //从频域转化为时域信号
    Dft dft(N);
    fftbuffer = dft.ifft2(Y);

    //去Zero-padding 得到真正的时域信号
    for(std::size_t i = 0; i < hM2; i++){
      y[i] = fftbuffer[N - hM2+i];
    }

    for(std::size_t i=hM2,k=0; i < M; i++,k++){
      y[i]=fftbuffer[k];
    }
    return y;
  }

}
