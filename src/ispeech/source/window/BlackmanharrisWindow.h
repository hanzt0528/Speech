/**
 * @file BlackmanWindow.h
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
 * @date 2007-2014
 * @license http://www.opensource.org/licenses/mit-license.php MIT
 * @since 0.0.1
 */

#ifndef BLACKMANHARRISWINDOW_H
#define BLACKMANHARRISWINDOW_H

#include "../../global.h"
#include "../SignalSource.h"
#include <cstddef>

namespace vfx_ispeech
{
    /**
     * Blackmanharris window.
     */
    class ISPEECH_EXPORT BlackmanharrisWindow : public SignalSource
    {
    public:
	BlackmanharrisWindow(std::size_t size);
    };
}

#endif // BLACKMANHARRISWINDOW_H
