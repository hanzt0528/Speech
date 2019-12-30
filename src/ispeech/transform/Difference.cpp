/**
 * @file OouraFft.cpp
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

#include "Difference.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include "../utils/log.h"
namespace vfx_ispeech
{
    /**
     * Initializes the transform for a given input length.
     *
     * Prepares the work area for PitchDetection algorithm.
     *
     * @param length input signal size (usually a power of 2)
     */
    Difference::Difference(std::size_t length)
    {
      N=length;
    }

    /**
     * Destroys the transform object and cleans up working area.
     */
    Difference::~Difference()
    {
    }

    /**
     * Applies the Time-domain Autocorrelation to the signal.
     *
     * @param x input signal
     * @return calculated Pitch
     */
  double Difference::Detect(const SampleType x[],uint64_t fs,double &pitch,uint64_t &position)
  {
    //LOG_WRN(this,"PitchDetection::Detect N = %lld",N);

    pitch = 0.0;
    position = 0;
    // create a temporary storage array and save the correlation of k.
    double* a = new double[N];
    memset(a,0,sizeof(double)*N);
    for (std::size_t k = 0; k < N; k++)
      {
	for(std::size_t i = 0; i < N-k; i++)
	  {
	    a[k]+=x[i]*x[i+k];
	  }
	//LOG_WRN(this,"a[%lld] = %f",k,x[k]);

      }

    //Detect the Pitch of input signal.
    double kmax = 0.0;

    uint64_t pos = 0;
    for(std::size_t k = 0; k < N; k++)
      {
	if(a[k]<0)
	  {
	    for(std::size_t i = k+1; i < N; i++)
	      {
		if(a[i] > kmax)
		  {
		    kmax = a[i];
		    pos = i;
		  }
	      }
	    break;
	  }
      }
    //LOG_WRN(this,"***********kmax = %f",kmax);
    //Interpolate peak values using parabolic interpolation

    double val = a[pos];
    double lval = a[pos-1];
    double rval = a[pos+1];
    double ipos = pos+0.5*(lval-rval)/(lval -2*val+rval);

    ipos = pos;

    delete[] a;

    if(ipos!=0)
      pitch = fs/ipos;

    position = uint64_t(ipos);
    return pitch;
  }
}
