/**
 * @file SquareGenerator.cpp
 *
 * Square wave generator.
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

#include "SquareGenerator.h"
#include <cmath>

namespace vfx_ispeech
{
    /**
     * Creates the generator object.
     *
     * @param sampleFrequency sample frequency of the signal
     */
    SquareGenerator::SquareGenerator(FrequencyType sampleFrequency):
	Generator(sampleFrequency), m_duty(0.5)
    {
    }

    /**
     * Fills the buffer with generated square samples.
     *`
     * @param samplesCount how many samples to generate
     */
    void SquareGenerator::generate(std::size_t samplesCount)
    {
	m_data.resize(samplesCount);
	std::size_t samplesPerPeriod = static_cast<std::size_t>(
	    m_sampleFrequency / static_cast<double>(m_frequency));
	std::size_t positiveLength = static_cast<std::size_t>(m_duty *
							      samplesPerPeriod);

	for (std::size_t i = 0; i < samplesCount; ++i)
	{
	    std::size_t t = i % samplesPerPeriod;
	    m_data[i] = m_amplitude * (t < positiveLength ? 1 : -1);
	}
    }
}
