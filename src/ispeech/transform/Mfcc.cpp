/**
 * @file Mfcc.cpp
 *
 * Calculation of MFCC signal features.
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

#include "Mfcc.h"
#include "Dct.h"
#include "../source/SignalSource.h"
#include "../filter/MelFilterBank.h"

namespace vfx_ispeech
{
    /**
     * Calculates a set of MFCC features from a given source.
     *
     * @param source input signal
     * @param numFeatures how many features to calculate
     * @return vector of MFCC features of length numFeatures
     */
    std::vector<double> Mfcc::calculate(const SignalSource &source,
					std::size_t numFeatures)
    {
	auto spectrum = m_fft->fft(source.toArray());

	MelFilterBank bank(source.getSampleFrequency(), m_inputSize);
	auto filterOutput = bank.applyAll(spectrum);

	Dct dct;
	return dct.dct(filterOutput, numFeatures);
    }
}
