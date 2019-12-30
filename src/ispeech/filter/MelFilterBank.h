/**
 * @file MelFilterBank.h
 *
 * A bank of number of Mel frequency triangular filters.
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

#ifndef MELFILTERBANK_H
#define MELFILTERBANK_H

#include "../global.h"
#include "MelFilter.h"
#include <cstddef>
#include <vector>

namespace vfx_ispeech
{
    /**
     * A wrapper class for a vector of triangular filters.
     */
    class ISPEECH_EXPORT MelFilterBank
    {
    public:
	MelFilterBank(FrequencyType sampleFrequency, std::size_t length,
		      FrequencyType melFilterWidth = 200.0,
		      std::size_t bankSize = 24);

	std::vector<double> applyAll(const SpectrumType &frameSpectrum) const;

	/**
	 * Returns sample frequency of all filters.
	 *
	 * @return sample frequency in Hz
	 */
	FrequencyType getSampleFrequency() const { return m_sampleFrequency; }

	/**
	 * Returns spectrum size of any filter spectra.
	 *
	 * @return spectrum size
	 */
	std::size_t getSpectrumLength() const { return N; }

	/**
	 * Returns the number of filters in bank.
	 *
	 * @return number of filters
	 */
	std::size_t size() const { return m_filters.size(); }

    private:
	/**
	 * Vector of Mel filters.
	 */
	std::vector<MelFilter> m_filters;

	/**
	 * Sample frequency of the filtered signal.
	 */
	FrequencyType m_sampleFrequency;

	/**
	 * Filter spectrum size (equal to zero-padded length of signal frame).
	 */
	std::size_t N;
    };
}
#endif // MELFILTERBANK_H
