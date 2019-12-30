/**
 * @file BarlettWindow.h
 *
 * Barlett (triangular) window.
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

#ifndef BARLETTWINDOW_H
#define BARLETTWINDOW_H

#include "../../global.h"
#include "../SignalSource.h"
#include <cstddef>

namespace vfx_ispeech
{
    /**
     * Barlett (triangular) window.
     */
    class ISPEECH_EXPORT BarlettWindow : public SignalSource
    {
    public:
	BarlettWindow(std::size_t size);
    };
}

#endif // BARLETTWINDOW_H
