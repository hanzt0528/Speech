/**
 * @file SignalSource.cpp
 *
 * A base signal source class.
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

#include "SignalSource.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <utility>


namespace vfx_ispeech
{


  /**
   * Take self sample shift to left on position.
   *
   * @param  position,how much shift to be.
   * @return void
   */

  void SignalSource::shiftLeft(std::size_t position)
  {

    std::size_t size = m_data.size();

      position > size?position = size:0;
      if(position == 0) return;

      std::vector<SampleType> data(size);

      std::transform(
		     std::begin(data),
		     std::end(data),
		     std::begin(data),
		     [] (SampleType x) { return 0.0; }
		     );



      auto si=begin();
      std::size_t i = 0;
      for(i = 0; i < position; i++)
	si++;

      i = 0;
      while(si != end()){
	data[i++]=*(si++);
      }

      std::transform(
		     std::begin(data),
		     std::end(data),
		     std::begin(m_data),
		     [] (SampleType x) { return x; }
		     );
    }
  /**
   * Take self sample shift to right on position.
   *
   * @param  position,how much shift to be.
   * @return void
   */
  void SignalSource::shiftRight(std::size_t position)
    {
      std::size_t size = m_data.size();

      position > size?position = size:0;
      if(position == 0) return;

      std::vector<SampleType> data(size);

      std::transform(
		     std::begin(data),
		     std::end(data),
		     std::begin(data),
		     [] (SampleType x) { return 0.0; }
		     );

      for(std::size_t i = position; i < size;i++){
	  data[i]=m_data[i-position];
      }
      for(std::size_t i = 0; i < size;i++){
	  m_data[i]=data[i];
      }

    }

  /**
   * Append a source to end of this object
   *
   * @param  a,the append object
   * @return void
   */
  void SignalSource::append(SignalSource & a)
  {
    std::size_t size = a.size();

    for(std::size_t i = 0; i < size; i++)
      {
	m_data.push_back(a.sample(i));
      }
  }

  /**
   * Append a source to font of this object
   *
   * @param  a,the append object
   * @return void
   */
  void SignalSource::appendHead(SignalSource & a)
  {
    m_data.insert(m_data.begin(),a.begin(),a.end());
  }

    /**
     * Add a constant value to each sample.
     *
     * @param x value to add
     * @return updated source
     */
    SignalSource& SignalSource::operator+=(SampleType x)
    {
	std::transform(
	    std::begin(m_data),
	    std::end(m_data),
	    std::begin(m_data),
	    [x] (SampleType y) { return x + y; }
	);
	return *this;
    }

    /**
     * Per-sample addition of other signal source.
     *
     * @param rhs source on the right-hand side of the operator
     * @return sum of two sources
     */
    SignalSource& SignalSource::operator+=(const SignalSource& rhs)
    {
	std::transform(
	    std::begin(m_data),
	    std::end(m_data),
	    std::begin(rhs.m_data),
	    std::begin(m_data),
	    [] (SampleType x, SampleType y) { return x + y; }
	);
	return *this;
    }

    /**
     * Multiply each sample by a constant value.
     *
     * @param x multiplier
     * @return updated source
     */
    SignalSource& SignalSource::operator*=(SampleType x)
    {
	std::transform(
	    std::begin(m_data),
	    std::end(m_data),
	    std::begin(m_data),
	    [x] (SampleType y) { return x * y; }
	);
	return *this;
    }

    /**
     * Per-sample multiplication with other signal source.
     *
     * @param rhs source on the right-hand side of the operator
     * @return product of two sources
     */
    SignalSource& SignalSource::operator*=(const SignalSource& rhs)
    {
	std::transform(
	    std::begin(m_data),
	    std::end(m_data),
	    std::begin(rhs.m_data),
	    std::begin(m_data),
	    [] (SampleType x, SampleType y) { return x * y; }
	);
	return *this;
    }
    SignalSource& SignalSource::operator/=(SampleType x)
    {
	std::transform(
	    std::begin(m_data),
	    std::end(m_data),
	    std::begin(m_data),
	    [x] (SampleType y) { return  y/x; }
	);
	return *this;
    }

    SignalSource& SignalSource::operator/=(const SignalSource& rhs)
    {
	      std::transform(
	    std::begin(m_data),
	    std::end(m_data),
	    std::begin(rhs.m_data),
	    std::begin(m_data),
	    [] (SampleType x, SampleType y) { return x / y; }
	);
	return *this;
    }



    SignalSource operator+(const SignalSource& lhs, SampleType x)
    {
	SignalSource result(lhs);
	return result += x;
    }

    SignalSource operator+(SignalSource&& lhs, SampleType x)
    {
	lhs += x;
	return std::move(lhs);
    }

    SignalSource operator+(SampleType x, const SignalSource& rhs)
    {
	SignalSource result(rhs);
	return result += x;
    }

    SignalSource operator+(SampleType x, SignalSource&& rhs)
    {
	rhs += x;
	return std::move(rhs);
    }

    SignalSource operator+(const SignalSource& lhs, const SignalSource& rhs)
    {
	SignalSource result(lhs);
	return result += rhs;
    }

    SignalSource operator+(SignalSource&& lhs, const SignalSource& rhs)
    {
	lhs += rhs;
	return std::move(lhs);
    }

    SignalSource operator+(const SignalSource& lhs, SignalSource&& rhs)
    {
	rhs += lhs;
	return std::move(rhs);
    }

    SignalSource operator*(const SignalSource& lhs, SampleType x)
    {
	SignalSource result(lhs);
	return result *= x;
    }

    SignalSource operator*(SignalSource&& lhs, SampleType x)
    {
	lhs *= x;
	return std::move(lhs);
    }

    SignalSource operator*(SampleType x, const SignalSource& rhs)
    {
	SignalSource result(rhs);
	return result *= x;
    }

    SignalSource operator*(SampleType x, SignalSource&& rhs)
    {
	rhs *= x;
	return std::move(rhs);
    }

    SignalSource operator*(const SignalSource& lhs, const SignalSource& rhs)
    {
	SignalSource result(lhs);
	return result *= rhs;
    }

    SignalSource operator*(SignalSource&& lhs, const SignalSource& rhs)
    {
	lhs *= rhs;
	return std::move(lhs);
    }

    SignalSource operator*(const SignalSource& lhs, SignalSource&& rhs)
    {
	rhs *= lhs;
	return std::move(rhs);
    }
    /**
     * Calculates sum value of the signal.
     *
     * @param source signal source
     * @return signal sum
     */
    double sum(const SignalSource& source)
    {
	double sum = std::accumulate(std::begin(source), std::end(source), 0.0);
	return sum;
    }

    /**
     * Calculates mean value of the signal.
     *
     * @param source signal source
     * @return signal mean
     */
    double mean(const SignalSource& source)
    {
	double sum = std::accumulate(std::begin(source), std::end(source), 0.0);
	return sum / source.size();
    }

    /**
     * Calculates energy of the signal.
     *
     * @param source signal source
     * @return signal energy
     */
    double energy(const SignalSource& source)
    {
	return std::accumulate(
	    std::begin(source),
	    std::end(source),
	    0.0,
	    [] (double acc, SampleType value) {
		return acc + value * value;
	    }
	);
    }

    /**
     * Calculates power of the signal.
     *
     * @param source signal source
     * @return signal power
     */
    double power(const SignalSource& source)
    {
	return energy(source) / source.size();
    }

    /**
     * Calculates Euclidean (L2) norm of the signal.
     *
     * @param source signal source
     * @return norm
     */
    double norm(const SignalSource& source)
    {
	return std::sqrt(energy(source));
    }

    /**
     * Calculates root mean square level of the signal.
     *
     * @param source signal source
     * @return RMS level
     */
    double rms(const SignalSource& source)
    {
	return std::sqrt(power(source));
    }
}
