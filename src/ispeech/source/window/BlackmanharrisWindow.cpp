/**
 * @file BlackmanharrisWindow.cpp
 *
 * Blackman window.
 *
 * This file is part of the ispeech library.
 * ispeech is iqiyi software, licensed under the MIT/X11 License. A copy of
 * the license is provided with the library in the LICENSE file.
 *
 * @package vfx_ispeech
 * @version 0.0.1-dev
 * @author Han Zhongtao
 * @date 23.9.2019
 * @license http://www.opensource.org/licenses/mit-license.php MIT
 * @since 0.0.1
 */

#include "BlackmanharrisWindow.h"
#include <cmath>

namespace vfx_ispeech
{
    /**
     * Creates Blackmanharris window of given size.
     *
     * @param size window length
     */
    BlackmanharrisWindow::BlackmanharrisWindow(std::size_t size):
	SignalSource()
    {
	m_data.reserve(size);
	for (std::size_t n = 0; n < size; ++n)
	{
	    m_data.push_back(
		0.35875 - 0.48829 * std::cos(2.0 * M_PI * n / double(size - 1)) + 0.14128 * std::cos(4.0 * M_PI * n / double(size - 1)) - 0.01168*std::cos(6.0 * M_PI * n / double(size - 1))
	    );
	}
    }
}
