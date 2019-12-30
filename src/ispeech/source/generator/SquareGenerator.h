/**
 * @file SquareGenerator.h
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

#ifndef SQUAREGENERATOR_H
#define SQUAREGENERATOR_H

#include "Generator.h"

namespace vfx_ispeech
{
    /**
     * Square wave generator.
     */
    class ISPEECH_EXPORT SquareGenerator : public Generator
    {
    public:
	SquareGenerator(FrequencyType sampleFrequency);

	virtual void generate(std::size_t samplesCount);

	/**
	 * Sets duty cycle of the generated square wave.
	 *
	 * Duty cycle is a fraction of period in which the signal is positive.
	 *
	 * @param duty duty cycle (0 < duty <= 1)
	 * @return the current object for fluent interface
	 */
	SquareGenerator& setDuty(double duty)
	{
	    m_duty = duty;

	    return *this;
	}

    private:
	/**
	 * Duty cycle, default = 0.5.
	 */
	double m_duty;
    };
}

#endif // SQUAREGENERATOR_H
