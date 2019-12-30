/**
 * @file PinkNoiseGenerator.cpp
 *
 * Pink noise generator.
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

#include "PinkNoiseGenerator.h"
#include "../../functions.h"

namespace vfx_ispeech
{
    /**
     * Creates the generator object.
     *
     * @param sampleFrequency sample frequency of the signal
     */
    PinkNoiseGenerator::PinkNoiseGenerator(FrequencyType sampleFrequency):
	Generator(sampleFrequency), key(0), maxKey(0xFFFF)
    {
    }

    /**
     * Fills the buffer with generated pink noise samples.
     *
     * @param samplesCount how many samples to generate
     */
    void PinkNoiseGenerator::generate(std::size_t samplesCount)
    {
	m_data.resize(samplesCount);

	// Voss algorithm initialization
	maxKey = 0xFFFF;
	key = 0;
	for (std::size_t i = 0; i < whiteSamplesNum; ++i)
	{
	    whiteSamples[i] = randomDouble() - 0.5;
	}

	for (std::size_t i = 0; i < samplesCount; ++i)
	{
	    m_data[i] = m_amplitude * pinkSample();
	}
    }

    /**
     * Generates a single pink noise sample using Voss algorithm.
     *
     * @return pink noise sample
     */
    double PinkNoiseGenerator::pinkSample()
    {
	int lastKey = key;
	double sum = 0;

	key++;
	if (key > maxKey)
	    key = 0;

	int diff = lastKey ^ key;
	for (std::size_t i = 0; i < whiteSamplesNum; ++i)
	{
	    if (diff & (1 << i))
		whiteSamples[i] = randomDouble() - 0.5;
	    sum += whiteSamples[i];
	}

	return sum / whiteSamplesNum;
    }
}
