/**
 * @file FftFactory.h
 *
 * A factory class to manage the creation of FFT calculation objects.
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

#ifndef FFTFACTORY_H
#define FFTFACTORY_H

#include "../global.h"
#include "Fft.h"
#include <cstddef>
#include <memory>

namespace vfx_ispeech
{
    /**
     * A factory class to manage the creation of FFT calculation objects.
     */
    class ISPEECH_EXPORT FftFactory
    {
    public:
	static std::shared_ptr<Fft> getFft(std::size_t length);
    };
}

#endif // FFTFACTORY_H
