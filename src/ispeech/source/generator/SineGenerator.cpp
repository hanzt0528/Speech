/**
 * @file SineGenerator.cpp
 *
 * Sine wave generator.
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

#include "SineGenerator.h"
#include <cmath>

namespace vfx_ispeech
{
    /**
     * Creates the generator object.
     *
     * @param sampleFrequency sample frequency of the signal
     */
    SineGenerator::SineGenerator(FrequencyType sampleFrequency):
	Generator(sampleFrequency)
    {
    }

    /**
     * Fills the buffer with generated sine samples.
     *
     * @param samplesCount how many samples to generate
     */
    void SineGenerator::generate(std::size_t samplesCount)
    {
	m_data.resize(samplesCount);
	double normalizedFrequency = m_frequency /
				     static_cast<double>(m_sampleFrequency);
	for (std::size_t i = 0; i < samplesCount; ++i)
	{
	    m_data[i] = m_amplitude * std::sin(
		2.0 * M_PI * normalizedFrequency * i +
		m_phase * 2.0 * M_PI
	    );
	}
    }
}
