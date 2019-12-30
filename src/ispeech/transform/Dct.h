/**
 * @file Dct.h
 *
 * Discrete Cosine Transform (DCT) calculation.
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

#ifndef DCT_H
#define DCT_H

#include "../global.h"
#include <cstddef>
#include <map>
#include <utility>
#include <vector>

namespace vfx_ispeech
{
    /**
     * An implementation of the Discrete Cosine Transform.
     */
    class ISPEECH_EXPORT Dct
    {
    public:
	/**
	 * Initializes the transform.
	 */
	Dct():
	    cosineCache()
	{
	}

	/**
	 * Destroys the transform object.
	 */
	~Dct()
	{
	    clearCosineCache();
	}

	std::vector<double> dct(const std::vector<double>& data, std::size_t outputLength);

    private:
	/**
	 * Key type for the cache, using input and output length.
	 */
	typedef std::pair<std::size_t, std::size_t> cosineCacheKeyType;

	/**
	 * Cache type.
	 */
	typedef std::map<cosineCacheKeyType, double**> cosineCacheType;

	/**
	 * Cache object, implemented as a map.
	 */
	cosineCacheType cosineCache;

	double** getCachedCosines(std::size_t inputLength, std::size_t outputLength);

	void clearCosineCache();
    };
}

#endif // DCT_H
