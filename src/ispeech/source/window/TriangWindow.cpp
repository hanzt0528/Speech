/**
 * @file TriangWindow.cpp
 *
 * Hann window.
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

#include "TriangWindow.h"
#include <cmath>

namespace vfx_ispeech
{
    /**
     * Creates Hann window of given size.
     *
     * @param size window length
     */
    TriangWindow::TriangWindow(std::size_t size):
	SignalSource()
    {
	m_data.reserve(size);
	for (std::size_t n = 0; n < size; ++n)
	{
	  m_data.push_back( n<(size/2.0)?(2*n+1)/double(size) : (2*(size-n)-1)/double(size));

	}
    }
}
