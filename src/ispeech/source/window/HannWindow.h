/**
 * @file HannWindow.h
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
 * @date 2019 - 2019
 * @license http://www.opensource.org/licenses/mit-license.php MIT
 * @since 0.0.1
 */

#ifndef HANNWINDOW_H
#define HANNWINDOW_H

#include "../../global.h"
#include "../SignalSource.h"
#include <cstddef>

namespace vfx_ispeech
{
    /**
     * Hann window.
     */
    class ISPEECH_EXPORT HannWindow : public SignalSource
    {
    public:
	HannWindow(std::size_t size);
    };
}

#endif // HANNWINDOW_H
