/**
 * @file global.h
 *
 * Global library typedefs and constants.
 *
 * This file is part of the ispeech library.
 * ispeech is software, licensed under the MIT/X11 License. A copy of
 * the license is provided with the library in the LICENSE file.
 *
 * @package vfx_ispeech
 * @version 0.0.1-dev
 * @author Han Zhongtao
 * @date 2019-2019
 * @license
 * @since 0.0.1
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <complex>
#include <vector>
#if defined (_WIN32) && defined(BUILD_SHARED_LIBS)
#  if defined(iSpeech_EXPORTS)
#    define ISPEECH_EXPORT  __declspec(dllexport)
#  else
#    define ISPEECH_EXPORT  __declspec(dllimport)
#  endif
#else
#    define ISPEECH_EXPORT
#endif

/**
 * Main library namespace.
 */
namespace vfx_ispeech
{
    /**
     * Library version in an easily comparable format.
     */
    const long VERSION_NUMBER = 0x300000;

    /**
     * Library version as a string.
     */
    const char* const VERSION_STRING = "0.0.1-dev";

    /**
     * Sample value type.
     */
    typedef double SampleType;

    /**
     * Sample frequency type.
     */
    typedef double FrequencyType;

    typedef std::size_t Position;

    typedef std::vector<Position> PositionListType;

    /**
     * Our standard complex number type, using double precision.
     */
    typedef std::complex<double> ComplexType;


    /**
     * Spectrum type - a vector of complex values.
     */
    typedef std::vector<ComplexType> SpectrumType;


    /**
     * Spectrum type - a vector of sample values.
     */
    typedef std::vector<SampleType> SampleListType;

    static const ComplexType j1(0,1);

    static const double LOC(std::pow(10,-5));

    typedef struct Analysis
    {
      SampleListType mX;
      SampleListType pX;
    }AnalysisType;

    typedef struct PeakInterpolate
    {
      SampleListType iploc;
      SampleListType ipmag;
      SampleListType ipphase;
    }PeakInterpolateType;

    //frequency, magnitude and phase of tracks
    typedef struct SineTracking
    {
      SampleListType tfreqn;
      SampleListType tmagn;
      SampleListType tphasen;
    }SineTrackingType;

    typedef struct Harmonics
    {
      SampleListType hfreq;
      SampleListType hmag;
      SampleListType hphase;
    }HarmonicsType;

    typedef struct TupleList
    {
      SampleListType indices;
      SampleListType elements;
    }TupleListType;

    typedef std::vector<SampleListType> SampleList2DType;


    typedef struct SoundDescriptor
    {
      double pitch;
      double energy;
      double rms;
      double power;
      double start;
      double end;
    }SoundDescriptorType;

    typedef struct MIDIDescriptor
    {
      double note;
      double start;
      double end;
    }MIDIDescriptorType;

}

#endif // GLOBAL_H
