/**
 * @file gen_spec_sines.h
 *
 * A Function class.
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

#ifndef gen_spec_sines_H
#define gen_spec_sines_H

#include "../global.h"


namespace vfx_ispeech
{
  /**
   * Returns a complex data spectrum value.
   *
   * @param x input signal
   * @param w analysis window
   * @param N FFT size
   * @return spectrum value at given frame and peak number
   */
  ISPEECH_EXPORT void genspecsines(SampleListType &iploc, SampleListType &ipmag, SampleListType &ipphase, int n_peaks, SampleListType &real, SampleListType &imag, int size_spec);

}

#endif // gen_spec_sines_H
