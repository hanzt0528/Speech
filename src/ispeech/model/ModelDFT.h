/**
 * @file ModelDFT.h
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

#ifndef ModelDFT_H
#define ModelDFT_H

#include "../ispeech.h"


namespace vfx_ispeech
{
    /**
     * A Model for the DFT algorithm
     */
    class ISPEECH_EXPORT ModelDFT
    {
    public:
	ModelDFT();
	~ModelDFT();
	 /**
	 * Returns a complex data spectrum value.
	 *
	 * @param x input signal
	 * @param w analysis window
	 * @param N FFT size
	 * @return spectrum value at given frame and peak number
	 */
	SampleListType dftModel(SignalSource &x,SignalSource &w,std::size_t N);
	 /**
	 * Returns the mugnitude and phase of signal at frequency-domain.
	 *
	 * @param x input signal
	 * @param w analysis window
	 * @param N FFT size
	 * @return AnalysisType mX is the mugnitude of spectrum,pX is the phase of spectrum.
	 */
	AnalysisType Analysis(SignalSource &x,SignalSource &w,std::size_t N);

	 /**
	 * Returns the synthesis signal.
	 *
	 * @param mX magnitude spectrum
	 * @param pX phase spectrum
	 * @param M window size
	 * @return synthesis of a signal using the discrete Fourier transfom
	 */
	SampleListType Synthesis(SampleListType mX,SampleListType pX,std::size_t M);


    };
}

#endif // ModelDFT_H
