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

#ifndef OOURAFFT_H
#define OOURAFFT_H

#include "Fft.h"

extern "C" {
    void cdft(int, int, double *, int *, double *);
    void rdft(int, int, double *, int *, double *);
}

namespace vfx_ispeech
{
    /**
     * A wrapper for the FFT algorithm found in Ooura mathematical packages.
     */
    class ISPEECH_EXPORT OouraFft : public Fft
    {
    public:
	OouraFft(std::size_t length);
	~OouraFft();

	virtual SpectrumType fft(const SampleType x[]);
	virtual void ifft(SpectrumType spectrum, double x[]);

	/**
	 * fft for pybind11.
	 */
	SpectrumType fft2(std::vector<SampleType> x);
	std::vector<SampleType> ifft2(SpectrumType spectrum);
    private:
	/**
	 * Work area for bit reversal.
	 */
	int* ip;

	/**
	 * Cos/sin table.
	 */
	double* w;
    };
}

#endif // OOURAFFT_H
