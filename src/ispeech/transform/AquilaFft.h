/**
 * @file AquilaFft.h
 *
 * A custom implementation of FFT radix-2 algorithm.
 *
 * This file is part of the Aquila DSP library.
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

#ifndef AQUILAFFT_H
#define AQUILAFFT_H

#include "Fft.h"
#include <map>
#include <utility>

namespace vfx_ispeech
{
    /**
     * ln(2) - needed for calculating number of stages in FFT.
     */
    const double LN_2 = 0.69314718055994530941723212145818;

    /**
     * A custom implementation of FFT radix-2 algorithm.
     */
    class ISPEECH_EXPORT AquilaFft : public Fft
    {
    public:
	/**
	 * Initializes the transform for a given input length.
	 *
	 * @param length input signal size (usually a power of 2)
	 */
	AquilaFft(std::size_t length):
	    Fft(length), fftWiCache()
	{
	}

	/**
	 * Destroys the transform object.
	 *
	 * @todo clear the cache somewhere else, not here!
	 */
	~AquilaFft()
	{
	    clearFftWiCache();
	}

	virtual SpectrumType fft(const SampleType x[]);
	virtual void ifft(SpectrumType spectrum, double x[]);

    private:
	/**
	 * Complex unit (0.0 + 1.0j).
	 */
	static const ComplexType j;

	/**
	 * Type of the twiddle factor cache key.
	 */
	typedef unsigned int fftWiCacheKeyType;

	/**
	 * Type of the twiddle factor cache.
	 */
	typedef std::map<fftWiCacheKeyType, ComplexType**> fftWiCacheType;

	/**
	 * Twiddle factor cache - implemented as a map.
	 */
	fftWiCacheType fftWiCache;

	ComplexType** getCachedFftWi(unsigned int numStages);

	void clearFftWiCache();
    };
}

#endif // AQUILAFFT_H
