/**
 * @file HammingWindow.h
 *
 * Hamming window.
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

#ifndef HAMMINGWINDOW_H
#define HAMMINGWINDOW_H

#include "../../global.h"
#include "../SignalSource.h"
#include <cstddef>

namespace vfx_ispeech
{
    /**
     * Hamming window.
     */
    class ISPEECH_EXPORT HammingWindow : public SignalSource
    {
    public:
	HammingWindow(std::size_t size);
    };
}

#endif // HAMMINGWINDOW_H
