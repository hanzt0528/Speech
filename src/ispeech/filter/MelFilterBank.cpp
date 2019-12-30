/**
 * @file MelFilterBank.cpp
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

#include "MelFilterBank.h"

namespace vfx_ispeech
{
    /**
     * Creates all the filters in the bank.
     *
     * @param sampleFrequency sample frequency in Hz
     * @param length spectrum size of each filter
     * @param melFilterWidth filter width in Mel frequency scale
     * @param bankSize number of filters in the bank
     */
    MelFilterBank::MelFilterBank(FrequencyType sampleFrequency,
				 std::size_t length,
				 FrequencyType melFilterWidth,
				 std::size_t bankSize):
	m_filters(), m_sampleFrequency(sampleFrequency), N(length)
    {
	m_filters.reserve(bankSize);
	for (std::size_t i = 0; i < bankSize; ++i)
	{
	    m_filters.push_back(MelFilter(m_sampleFrequency));
	    m_filters[i].createFilter(i, melFilterWidth, N);
	}
    }

    /**
     * Processes frame spectrum through all filters.
     *
     * @param frameSpectrum frame spectrum
     * @return vector of results (one value per each filter)
     */
    std::vector<double> MelFilterBank::applyAll(const SpectrumType& frameSpectrum) const
    {
	std::vector<double> output(size(), 0.0);
	for (std::size_t i = 0; i < size(); ++i)
	{
	    output[i] = m_filters[i].apply(frameSpectrum);
	}
	return output;
    }
}
