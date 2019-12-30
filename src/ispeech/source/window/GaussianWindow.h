/**
 * @file GaussianWindow.h
 *
 * Gaussian (triangular) window. Based on the formula given at:
 * http://en.wikipedia.org/wiki/Window_function#Gaussian_window
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

#ifndef GAUSSIANWINDOW_H
#define GAUSSIANWINDOW_H

#include "../../global.h"
#include "../SignalSource.h"
#include <cstddef>

namespace vfx_ispeech
{
    /**
     * Creates Gaussian window of given size, with optional sigma parameter.
     */
    class ISPEECH_EXPORT GaussianWindow : public SignalSource
    {
    public:
	GaussianWindow(std::size_t size, double sigma = 0.5);
    };
}

#endif // GAUSSIANWINDOW_H
