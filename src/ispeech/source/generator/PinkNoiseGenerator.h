/**
 * @file PinkNoiseGenerator.h
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

#ifndef PINKNOISEGENERATOR_H
#define PINKNOISEGENERATOR_H

#include "Generator.h"

namespace vfx_ispeech
{
    /**
     * Pink noise generator using Voss algorithm.
     */
    class ISPEECH_EXPORT PinkNoiseGenerator : public Generator
    {
    public:
	PinkNoiseGenerator(FrequencyType sampleFrequency);

	virtual void generate(std::size_t samplesCount);

    private:
	double pinkSample();

	/**
	 * Number of white noise samples to use in Voss algorithm.
	 */
	enum { whiteSamplesNum = 20 };

	/**
	 * An internal buffer for white noise samples.
	 */
	double whiteSamples[whiteSamplesNum];

	/**
	 * A key marking which of the white noise samples should change.
	 */
	int key;

	/**
	 * Maximum key value.
	 */
	int maxKey;
    };
}

#endif // PINKNOISEGENERATOR_H
