/**
 * @file Dft.cpp
 *
 * A reference implementation of the Discrete Fourier Transform.
 *
 * Note - this is a direct application of the DFT equations and as such it
 * surely isn't optimal. The implementation here serves only as a reference
 * model to compare with.
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

#include "Dft.h"
#include <algorithm>
#include <cmath>
#include <complex>

namespace vfx_ispeech
{
    /**
     * Complex unit.
     */
    const ComplexType Dft::j(0, 1);

    /**
     * Applies the transformation to the signal.
     *
     * @param x input signal
     * @return calculated spectrum
     */
    SpectrumType Dft::fft(const SampleType x[])
    {
	SpectrumType spectrum(N);
	ComplexType WN = std::exp((-j) * 2.0 * M_PI / static_cast<double>(N));

	for (unsigned int k = 0; k < N; ++k)
	{
	    ComplexType sum(0, 0);
	    for (unsigned int n = 0; n < N; ++n)
	    {
		sum += x[n] * std::pow(WN, n * k);
	    }
	    spectrum[k] = sum;
	}

	return spectrum;
    }

    /**
     * Applies the inverse transform to the spectrum.
     *
     * @param spectrum input spectrum
     * @param x output signal
     */
    void Dft::ifft(SpectrumType spectrum, double x[])
    {
	ComplexType WN = std::exp((-j) * 2.0 * M_PI / static_cast<double>(N));
	for (unsigned int k = 0; k < N; ++k)
	{
	    ComplexType sum(0, 0);
	    for (unsigned int n = 0; n < N; ++n)
	    {
		sum += spectrum[n] * std::pow(WN, -static_cast<int>(n * k));
	    }
	    //x[k] = std::abs(sum) / static_cast<double>(N);
	    x[k] = sum.real() / static_cast<double>(N);
	}
    }

  SpectrumType Dft::fft2(std::vector<SampleType> x)
  {
    SpectrumType spectrum(N);
    ComplexType WN = std::exp((-j) * 2.0 * M_PI / static_cast<double>(N));

    for (unsigned int k = 0; k < N; ++k)
      {
	ComplexType sum(0, 0);
	for (unsigned int n = 0; n < N; ++n)
	  {
	    sum += x[n] * std::pow(WN, n * k);
	  }
	spectrum[k] = sum;
      }

    return spectrum;
  }

  std::vector<SampleType> Dft::ifft2(SpectrumType spectrum)
  {
    std::vector<SampleType> x(N);

    ComplexType WN = std::exp((-j) * 2.0 * M_PI / static_cast<double>(N));
    for (unsigned int k = 0; k < N; ++k)
      {
	ComplexType sum(0, 0);
	for (unsigned int n = 0; n < N; ++n)
	  {
	    sum += spectrum[n] * std::pow(WN, -static_cast<int>(n * k));
	  }
	x[k] = sum.real() / static_cast<double>(N);
      }
    return x;
  }


}
