/**
 * @file SineGenerator.h
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

#ifndef SINEGENERATOR_H
#define SINEGENERATOR_H

#include "Generator.h"

namespace vfx_ispeech
{
    /**
     * Sine wave generator.
     */
    class ISPEECH_EXPORT SineGenerator : public Generator
    {
    public:
	SineGenerator(FrequencyType sampleFrequency);

	virtual void generate(std::size_t samplesCount);
    };
}

#endif // SINEGENERATOR_H
