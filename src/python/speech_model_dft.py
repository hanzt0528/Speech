import pdb
import numpy as np 
import matplotlib.pyplot as plt
import speech_fft as speech_fft
import math
tol = 1e-14

def isPower2(num):
    """
    Check if num is power of two
    """
    return ((num & (num - 1)) == 0) and num > 0

def dftAnal(x,w,N):
    if not(isPower2(N)):
        raise ValueError("FFT size(N) is not a pwer of 2")

    if(w.size > N):
        raise ValueError("Window size(M) is bigger than FFT size")

    hN = (N//2)+1
    # size of positive spectrum, it includes sample 0
    hM1 = (w.size+1)//2
    # half analysis window size by rounding
    hM2 = w.size//2
    # half analysis window size by floor
    fftbuffer = np.zeros(N)
    # initialize buffer for FFT
    w = w / sum(w)
    # normalize analysis window
    xw = x*w
    # window the input sound
    fftbuffer[:hM1] = xw[hM2:]
    # zero-phase window in fftbuffer
    fftbuffer[-hM2:] = xw[:hM2]        
    X = speech_fft.fft(fftbuffer,N)
    # compute FFT
    absX = abs(X[:hN])
    # compute ansolute value of positive side
    absX[absX<np.finfo(float).eps] = np.finfo(float).eps
    # if zeros add epsilon to handle log
    mX = 20 * np.log10(absX)
    # magnitude spectrum of positive frequencies in dB
    X[:hN].real[np.abs(X[:hN].real) < tol] = 0.0
    # for phase calculation set to 0 the small values
    X[:hN].imag[np.abs(X[:hN].imag) < tol] = 0.0
    # for phase calculation set to 0 the small values         
    pX = np.unwrap(np.angle(X[:hN]))
    # unwrapped phase spectrum of positive frequencies

    return mX, pX

def dftSynth(mX, pX, M):
    """
    Synthesis of a signal using the discrete Fourier transform
    mX: magnitude spectrum, pX: phase spectrum, M: window size
    returns y: output signal
    """

    # size of positive spectrum, it includes sample 0
    hN = mX.size

    # FFT size
    N = (hN-1)*2
    # raise error if N not a power of two, thus mX is wrong
    if not(isPower2(N)):
        raise ValueError("size of mX is not (N/2)+1")
    # half analysis window size by rounding
    hM1 = int(math.floor((M+1)/2))

    # half analysis window size by floor
    hM2 = int(math.floor(M/2))

    # initialize buffer for FFT
    fftbuffer = np.zeros(N)

    # initialize output array        
    y = np.zeros(M)                                  
    # clean output spectrum
    Y = np.zeros(N, dtype = complex)

    # generate positive frequencies
    Y[:hN] = 10**(mX/20) * np.exp(1j*pX)
    # generate negative frequencies
    Y[hN:] = 10**(mX[-2:0:-1]/20) * np.exp(-1j*pX[-2:0:-1])
    # compute inverse FFT
    #fftbuffer = np.real(ifft(Y))
    fftbuffer = np.real(speech_fft.ifft(Y,N)/N)
    # undo zero-phase window
    y[:hM2] = fftbuffer[-hM2:]
    y[hM2:] = fftbuffer[:hM1]
    return y
    
