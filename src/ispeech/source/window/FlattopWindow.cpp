/**
 * @file FlattopWindow.cpp
 *
 * Flat-top window.
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

#include "FlattopWindow.h"
#include <cmath>

namespace vfx_ispeech
{
    /**
     * Creates flat-top window of given size.
     *
     * @param size window length
     */
    FlattopWindow::FlattopWindow(std::size_t size):
	SignalSource()
    {
	m_data.reserve(size);
	for (std::size_t n = 0; n < size; ++n)
	{
	    m_data.push_back(
		1.0 - 1.93 * std::cos(2.0 * M_PI * n / double(size - 1)) +
		    1.29 * std::cos(4.0 * M_PI * n / double(size - 1)) -
		    0.388 * std::cos(6.0 * M_PI * n / double(size - 1)) +
		    0.0322 * std::cos(8.0 * M_PI * n / double(size - 1))
	    );
	}
    }
}
