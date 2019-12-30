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

#include "ModelSTFT.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <random>

namespace vfx_ispeech
{
  /**
   * Initializes a model.
   *
   * Prepares for a STFT model.
   *
   */
  ModelSTFT::ModelSTFT()
  {
  }

  /**
   * Destroys the model object and cleans up working area.
   */
  ModelSTFT::~ModelSTFT()
  {
  }

  /**
   * Analysis of a sound using the dft  model.
   *
   * @param x input array sound
   * @param fs sampling rate
   * @param w analysis window
   * @param N FFT size
   * @param H hop size
   * @return returns  xtmag, xtphase: magnitudes and phases of x
   */
  std::size_t ModelSTFT::Analysis(SignalSource &x,SignalSource &w,std::size_t N,std::size_t H)
  {
    if(H < 0){
      LOG_ERR(this,"ModelSTFT::Analysis Hop size (H) smaller or equal to 0,here H=%lld",H);
      return 0;
    }

    std::size_t M = w.size();
    std::size_t hM1 = (M + 1) / 2;//128
    std::size_t hM2 = (std::size_t)std::floor(M/2);//128

    SignalSource zeros(hM2);
    // add zeros at beginning to center first window at sample 0
    x.appendHead(zeros);
    // add zeros at the end to analyze last sample
    x.append(zeros);

    //init sound pointer in middle of anal window
    std::size_t pin = hM1;
    //last sample to start a frame
    std::size_t pend = x.size()- hM1;

    //normalize analysis window
    w/=sum(w);

    ModelDFT dft;

    xmX.clear();
    xpX.clear();

    while( pin <= pend){
      // select frame
      SignalSource x1(x.toArray()+pin-hM1,M);
      // compute dft
      AnalysisType a = dft.Analysis(x1,w,N);
      xmX.push_back(a.mX);
      xpX.push_back(a.pX);

      pin+=H;
    }

    return xmX.size();
  }

  std::size_t ModelSTFT::GetAnalysisTypeCount()
  {
    return xmX.size();
  }

  AnalysisType ModelSTFT::GetAnalysisTypeByIndex(std::size_t index)
  {
    AnalysisType  r;
    r.mX = xmX[index];
    r.pX = xpX[index];

    return r;
  }

  /**
   * Returns the synthesis signal.
   *
   * @param mY magnitude spectra
   * @param pY phase spectra
   * @param M Synthesis FFT size
   * @param H hop size
   * @return synthesis of a signal using the discrete Fourier transfom
   */
  SampleListType ModelSTFT::Synthesis(SampleList2DType &mY,SampleList2DType &pY,std::size_t M,std::size_t H)
  {
    // half analysis window size by rounding
    std::size_t hM1 = (M+1)/2;
      // half analysis window size by floor
    std::size_t hM2 = (std::size_t)std::floor(M/2);

    // number of frames
    std::size_t nFrames = mY.size();

    //initialize output array
    SampleListType y(nFrames*H + hM1 + hM2,0);

    std::size_t pin = hM1;

    ModelDFT dft;
    for(std::size_t l = 0; l < nFrames; l++)
      {
	SampleListType y1 = dft.Synthesis(mY[l],pY[l],M);

	for(std::size_t k = 0; k < y1.size();k++)
	  {
	    y[pin-hM1+k] += y1[k]*H;
	  }
	pin += H;
      }

    y.erase(y.begin(),y.begin()+hM2);
    y.erase(y.begin()+y.size()-hM1,y.end());

    return y;
  }
  SampleListType ModelSTFT::AnalysisAndSynthesis(SignalSource &x,SignalSource &w,std::size_t N,std::size_t H)
  {
    Analysis(x,w,N,H);
    LOG_ERR(nullptr,"ModelSTFT::AnalysisAndSynthesis Analysis done");

    std::size_t M = w.size();

    SampleListType y = Synthesis(xmX,xpX,M,H);

    LOG_ERR(nullptr,"ModelSTFT::AnalysisAndSynthesis Synthesis done");
    SampleListType result(y.begin(),y.end());

    LOG_ERR(nullptr,"ModelSTFT::AnalysisAndSynthesis Synthesis result size = %lld",result.size());

    return result;
  }

}
