/**
 * @file TriangleGenerator.h
 *
 * Triangle (and sawtooth) wave generator.
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

#ifndef TRIANGLEGENERATOR_H
#define TRIANGLEGENERATOR_H

#include "Generator.h"

namespace vfx_ispeech
{
    /**
     * Triangle (and sawtooth) wave generator.
     */
    class ISPEECH_EXPORT TriangleGenerator : public Generator
    {
    public:
	TriangleGenerator(FrequencyType sampleFrequency);

	virtual void generate(std::size_t samplesCount);

	/**
	 * Sets slope width of the generated triangle wave.
	 *
	 * Slope width is a fraction of period in which signal is rising.
	 *
	 * @param width slope width (0 < width <= 1)
	 * @return the current object for fluent interface
	 */
	TriangleGenerator& setWidth(double width)
	{
	    m_width = width;

	    return *this;
	}

    private:
	/**
	 * Slope width, default = 1.0 (generates sawtooth wave).
	 */
	double m_width;
    };
}

#endif // TRIANGLEGENERATOR_H
