/**
 * @file Generator.cpp
 *
 * An interface for signal generators.
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

#include "Generator.h"

namespace vfx_ispeech
{
    /**
     * Creates the generator object.
     *
     * @param sampleFrequency sample frequency of the signal
     */
    Generator::Generator(FrequencyType sampleFrequency):
	SignalSource(sampleFrequency), m_frequency(0), m_amplitude(0),
	m_phase(0.0)
    {
    }
  Dog::Dog(FrequencyType sampleFrequency) : Pet(sampleFrequency) { }

  std::string Dog::bark() { return "woof!"; }
}
