/**
 * @file PlainTextFile.h
 *
 * Reading samples from a plain text file.
 *
 * This file is part of the ispeech library.
 * ispeech is free software, licensed under the MIT/X11 License. A copy of
 * the license is provided with the library in the LICENSE file.
 *
 * @package vfx_ispeech
 * @version 0.0.1-dev
 * @author Han Zhongtao
 * @date 2019-2019
 * @license
 * @since 0.0.1
 */

#ifndef PLAINTEXTFILE_H
#define PLAINTEXTFILE_H

#include "../global.h"
#include "SignalSource.h"
#include <string>

namespace vfx_ispeech
{
    /**
     * Plain text file, where each sample is in new line.
     *
     * No headers are allowed in the file, only a simple list of numbers
     * will work at the moment.
     *
     * Any numeric type will be converted on the fly to SampleType. Sample
     * rate must be known prior to opening the file as the constructor expects
     * sample frequency as its second argument.
     */
    class ISPEECH_EXPORT PlainTextFile : public SignalSource
    {
    public:
	PlainTextFile(std::string filename, FrequencyType sampleFrequency);

	static void save(const SignalSource& source, const std::string& file);
    };
}

#endif // PLAINTEXTFILE_H
