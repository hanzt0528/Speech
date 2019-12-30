/**
 * @file WhiteNoiseGenerator.h
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

#ifndef WHITENOISEGENERATOR_H
#define WHITENOISEGENERATOR_H

#include "Generator.h"

namespace vfx_ispeech
{
    /**
     * White noise generator.
     */
    class ISPEECH_EXPORT WhiteNoiseGenerator : public Generator
    {
    public:
	WhiteNoiseGenerator(FrequencyType sampleFrequency);

	virtual void generate(std::size_t samplesCount);
    };
}

#endif // WHITENOISEGENERATOR_H
