/**
 * @file PlainTextFile.cpp
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

#include "PlainTextFile.h"
#include <algorithm>
#include <fstream>
#include <iterator>

namespace vfx_ispeech
{
    /**
     * Creates the data source.
     *
     * @param filename full path to .txt file
     * @param sampleFrequency sample frequency of the data in file
     */
    PlainTextFile::PlainTextFile(std::string filename,
				 FrequencyType sampleFrequency):
	SignalSource(sampleFrequency)
    {
	std::fstream fs;
	fs.open(filename.c_str(), std::ios::in);
	std::copy(std::istream_iterator<SampleType>(fs),
		  std::istream_iterator<SampleType>(),
		  std::back_inserter(m_data));
	fs.close();
    }

    /**
     * Saves the given signal source as a plain text file.
     *
     * @param source source of the data to save
     * @param filename destination file
     */
    void PlainTextFile::save(const SignalSource& source,
			     const std::string& filename)
    {
	std::fstream fs;
	fs.open(filename.c_str(), std::ios::out);
	std::copy(std::begin(source),
		  std::end(source),
		  std::ostream_iterator<SampleType>(fs, "\n"));
	fs.close();
    }
}
