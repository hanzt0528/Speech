/**
 * @file GaussianWindow.cpp
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
 * @date 2007-2014
 * @license http://www.opensource.org/licenses/mit-license.php MIT
 * @since 0.0.1
 */

#include "GaussianWindow.h"
#include <cmath>

namespace vfx_ispeech
{
    /**
     * Creates Gaussian window of given size, with optional sigma parameter.
     *
     * @param size window length
     * @param sigma standard deviation
     */
    GaussianWindow::GaussianWindow(std::size_t size, double sigma/* = 0.5*/):
	SignalSource()
    {
	m_data.reserve(size);
	for (std::size_t n = 0; n < size; ++n)
	{
	    m_data.push_back(
		std::exp((-0.5) * std::pow((n - (size - 1.0) / 2.0)/(sigma * (size - 1.0) / 2.0), 2.0))
	    );
	}
    }
}
