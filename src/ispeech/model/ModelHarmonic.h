/**
 * @file ModelHarmonic.h
 *
 * A Harmonic Model,it can analysis an audio signal to sequence of harmonics.
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

#ifndef ModelHarmonic_H
#define ModelHarmonic_H

#include "../ispeech.h"


namespace vfx_ispeech
{
  /**
   * A Model for the Harmonic algorithm
   */
  class ISPEECH_EXPORT ModelHarmonic
  {
  public:
    ModelHarmonic();
    ~ModelHarmonic();
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
    SampleListType f0Detection(SignalSource x,double fs,SignalSource w,std::size_t N,std::size_t H,double t,double minf0,double maxf0,double f0et);
    /**
     *  Detection of the harmonics of a frame from a set of spectral peaks using f0.
     *
     * @param pfreq peak frequencies
     * @param pmag magnitudes
     * @param pphase phases
     * @param f0 fundamental frequencies
     * @param nH number of harmonics
     * @param hfreqp harmonic frequencies of previous frame
     * @param fs sampling rate
     * @param harmDevSlope slop of change of the deviation allowed to perfect harmonic
     * @return HarmonicsType hfreq, hmag, hphase: harmonic frequencies, magnitudes, phases
     */
    HarmonicsType harmonicDetection(SampleListType &pfreq,SampleListType &pmag,SampleListType &pphase,double f0,std::size_t nH,SampleListType &hfreqp,double fs,double harmDevSlope = 0.01);

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
     * @return xhfreq, xhmag, xhphase: harmonic frequencies, magnitudes and phases,use GetAnalysisTypeByIndex to get these return things, do this way because of thiking about something,like python binding.
     */
    AnalysisType Analysis(SignalSource x,double fs,SignalSource w,std::size_t N,std::size_t H,double t,std::size_t nH,double minf0,double maxf0,double f0et,double harmDevSlope =0.01,double minSineDur=0.02);

    std::size_t GetAnalysisTypeCount();

    HarmonicsType GetAnalysisTypeByIndex(std::size_t index);

  protected:
    std::vector<SampleListType> xhfreq_;
    std::vector<SampleListType> xhmag_;
    std::vector<SampleListType> xhphase_;
  };
}

#endif // ModelHarmonic_H
