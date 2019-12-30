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

#ifndef ModelSine_H
#define ModelSine_H

#include "../ispeech.h"


namespace vfx_ispeech
{
    /**
     * A Model for the DFT algorithm
     */
    class ISPEECH_EXPORT ModelSine
    {
    public:
	ModelSine();
	~ModelSine();
	 /**
	 * Returns a complex data spectrum value.
	 *
	 * @param x input array sound
	 * @param fs sampling rate
	 * @param w analysis window
	 * @param N size of complex spectrum
	 * @param t threshold in nagative dB
	 * @return ouput array sound
	 */
	SampleListType sineModel(SignalSource &x,double fs,SignalSource &w,std::size_t N,double t);

	/**
	 * Analysis of a sound using the sinusoidal model with sine tracking.
	 *
	 * @param x input array sound
	 * @param fs sampling rate
	 * @param w analysis window
	 * @param N size of complex spectrum
	 * @param t threshold in nagative dB
	 * @param maxnSines maxinum number of sines per frame
	 * @param minSineDur mininum duration of sines in seconds
	 * @param freqDevoffset mininum frequency deviation at 0Hz
	 * @param freqDevSlope slope increase of mininum frequency deviation
	 * @return returns xtfreq, xtmag, xtphase:frequencies, magnitudes and phases of sinusoidal tracks
	 */
	AnalysisType Analysis(SignalSource &x,double fs,SignalSource &w,std::size_t N,std::size_t H,double t,std::size_t maxnSines = 100,double minSineDur=0.01,double freqDevoffset = 20.0,double freqDevSlope=0.01);


	std::size_t GetAnalysisTypeCount();

	SineTrackingType GetAnalysisTypeByIndex(std::size_t index);

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
	SampleListType Synthesis(SampleList2DType &tfreq,SampleList2DType &tmag,SampleList2DType tphase,std::size_t N,std::size_t H,double fs);

	 /**
	 * Returns the synthesis signal.this function for test the Synthesis function,because of some input params of Synthesis is not very happy to python script!
	 *
	 *
	 * @param x input array sound
	 * @param fs sampling rate
	 * @param w analysis window
	 * @param N size of complex spectrum
	 * @param t threshold in nagative dB
	 * @param maxnSines maxinum number of sines per frame
	 * @param minSineDur mininum duration of sines in seconds
	 * @param freqDevoffset mininum frequency deviation at 0Hz
	 * @param freqDevSlope slope increase of mininum frequency deviation       * @return synthesis of a signal using the discrete Fourier transfom
	 */
	SampleListType AnalysisAndSynthesis(SignalSource &x,double fs,SignalSource &w,std::size_t N,std::size_t H,double t,std::size_t maxnSines = 100,double minSineDur=0.01,double freqDevoffset = 20.0,double freqDevSlope=0.01);

    protected:
	std::vector<SampleListType> xtfreq;
	std::vector<SampleListType> xtmag;
	std::vector<SampleListType> xtphase;
    };
}

#endif // ModelSine_H
