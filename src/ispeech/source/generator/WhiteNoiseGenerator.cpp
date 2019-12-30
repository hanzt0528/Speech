/**
 * @file WhiteNoiseGenerator.cpp
 *
 * White noise generator.
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

#include "WhiteNoiseGenerator.h"
#include "../../functions.h"

namespace vfx_ispeech
{
    /**
     * Creates the generator object.
     *
     * @param sampleFrequency sample frequency of the signal
     */
    WhiteNoiseGenerator::WhiteNoiseGenerator(FrequencyType sampleFrequency):
	Generator(sampleFrequency)
    {
    }

    /**
     * Fills the buffer with generated white noise samples.
     *
     * @param samplesCount how many samples to generate
     */
    void WhiteNoiseGenerator::generate(std::size_t samplesCount)
    {
	m_data.resize(samplesCount);
	for (std::size_t i = 0; i < samplesCount; ++i)
	{
	    m_data[i] = m_amplitude * (randomDouble() - 0.5);
	}
    }
}
