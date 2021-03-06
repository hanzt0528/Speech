/**
 * @file Spectrogram.h
 *
 * Spectrogram calculation.
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

#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include "../global.h"
#include <cstddef>
#include <memory>
#include <vector>


namespace vfx_ispeech
{
    class Fft;
    class FramesCollection;

    /**
     * Spectrogram class.
     *
     * @todo copy constructor, safe point access, code cleanup
     */
    class ISPEECH_EXPORT Spectrogram
    {
    public:
	Spectrogram(FramesCollection& frames);

	/**
	 * Returns number of frames (spectrogram width).
	 *
	 * @return frame count
	 */
	std::size_t getFrameCount() const
	{
	    return m_frameCount;
	}

	/**
	 * Returns spectrum size (spectrogram height).
	 *
	 * @return spectrum size
	 */
	std::size_t getSpectrumSize() const
	{
	    return m_spectrumSize;
	}

	/**
	 * Returns a complex data point value at given coordinates.
	 *
	 * @param frame frame number (the x coordinate)
	 * @param peak spectral peak number (the y coordinate)
	 * @return spectrum value at given frame and peak number
	 */
	ComplexType getPoint(std::size_t frame, std::size_t peak) const
	{
	    return (*m_data)[frame][peak];
	}

    private:
	/**
	 * Spectrogram data type used for internal storage.
	 */
	typedef std::vector<SpectrumType> SpectrogramDataType;

	/**
	 * Frame count (width of the spectrogram).
	 */
	std::size_t m_frameCount;

	/**
	 * Spectrum size (height of the spectrogram).
	 */
	std::size_t m_spectrumSize;

	/**
	 * A shared pointer to FFT algorithm class.
	 */
	std::shared_ptr<Fft> m_fft;

	/**
	 * A shared pointer to spectrogram data.
	 */
	std::shared_ptr<SpectrogramDataType> m_data;
    };
}

#endif // SPECTROGRAM_H
