/**
 * @file ModelSine.h
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

#ifndef ModelSTFT_H
#define ModelSTFT_H

#include "../ispeech.h"


namespace vfx_ispeech
{
    /**
     * A Model for the DFT algorithm
     */
    class ISPEECH_EXPORT ModelSTFT
    {
    public:
	ModelSTFT();
	~ModelSTFT();
	/**
	 * Analysis of a sound using the dft  model.
	 *
	 * @param x input array sound
	 * @param fs sampling rate
	 * @param w analysis window
	 * @param N FFT size
	 * @param H hop size
	 * @return returns  count of mX and pX
	 */
	std::size_t Analysis(SignalSource &x,SignalSource &w,std::size_t N,std::size_t H);

	std::size_t GetAnalysisTypeCount();

	AnalysisType GetAnalysisTypeByIndex(std::size_t index);

	 /**
	 * Returns the synthesis signal.
	 *
	 * @param tfreq frequencies of sinusoids
	 * @param mY magnitude spectra
	 * @param pY phase spectra
	 * @param M Synthesis FFT size
	 * @param H hop size
	 * @return synthesis of a signal using the discrete Fourier transfom
	 */
	SampleListType Synthesis(SampleList2DType &mY,SampleList2DType &pY,std::size_t M,std::size_t H);

	 /**
	 * Returns the synthesis signal.this function for test the Synthesis function,because of some input params of Synthesis is not very happy to python script!
	 *
	 *
	 * @param x input array sound
	 * @param fs sampling rate
	 * @param N FFT size
	 * @param H hop size
on       * @return synthesis of a signal using the discrete Fourier transfom
	 */
	SampleListType AnalysisAndSynthesis(SignalSource &x,SignalSource &w,std::size_t N,std::size_t H);

    protected:
	std::vector<SampleListType> xmX;
	std::vector<SampleListType> xpX;
    };
}

#endif // ModelSTFT_H
