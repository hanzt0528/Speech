/**
 * @file OouraFft.h
 *
 * A wrapper for the FFT algorithm found in Ooura mathematical packages.
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

#ifndef PITCHDETECTION_H
#define PITCHDETECTION_H

#include "../global.h"


namespace vfx_ispeech
{
    /**
     * A wrapper for the FFT algorithm found in Ooura mathematical packages.
     */
    class ISPEECH_EXPORT PitchDetection
    {
    public:
	PitchDetection(std::size_t length);
	~PitchDetection();

	double Detect(const SampleType x[],uint64_t fs,double &pitch,uint64_t &position);
	double Detect2(const SampleType x[],uint64_t fs);

    private:
	std::size_t N;
    };
}

#endif // PITCHDETECTION_H
