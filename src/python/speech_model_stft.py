import numpy as np
import math
import matplotlib.pyplot as plt
from scipy.signal import triang, resample,blackmanharris
from scipy.fftpack import fft,fftshift,ifft

import speech_model_dft as speech_model_dft

def stftAnal(x, w, N, H):
    """
    Analysis of a sound using the short-time Fourier transform
    x: input array sound, w: analysis window, N: FFT size, H: hop size
    returns xmX, xpX: magnitude and phase spectra
    """
    # raise error if hop size 0 or negative
    if (H <= 0):
        raise ValueError("Hop size (H) smaller or equal to 0")
    # size of analysis window
    M = w.size

    # half analysis window size by rounding
    hM1 = (M+1)//2
    #hM1 = 128 of M = 256
    #print('hM1 = '+str(hM1))
    # half analysis window size by floor
    hM2 = M//2
    #hM2 = 128 of M=256
    #print('hM2 = '+str(hM2))

    # add zeros at beginning to center first window at sample 0
    x = np.append(np.zeros(hM2),x)
    # add zeros at the end to analyze last sample
    x = np.append(x,np.zeros(hM2))
    # initialize sound pointer in middle of analysis window
    pin = hM1
    # last sample to start a frame
    pend = x.size-hM1
    # normalize analysis window
    w = w / sum(w)
    # Initialise empty list for mX
    xmX = []
    # Initialise empty list for pX
    xpX = []
    # while sound pointer is smaller than last sample
    while pin<=pend:
        # select one frame of input sound
        x1 = x[pin-hM1:pin+hM2]
        #print('x1 size = '+ str(x1.size))
        #size of x1 equal 256 of M = 256
        # compute dft
        mX, pX = speech_model_dft.dftAnal(x1, w, N)
        # Append output to list
        xmX.append(np.array(mX))

        xpX.append(np.array(pX))
        # advance sound pointer
        pin += H
        #print('stftAnal pin = '+str(pin)+',pend='+str(pend))


    # Convert to numpy array
    xmX = np.array(xmX)
    xpX = np.array(xpX)
    return xmX, xpX

def stftSynth(mY, pY, M, H) :
    """
    Synthesis of a sound using the short-time Fourier transform
    mY: magnitude spectra, pY: phase spectra, M: window size, H: hop-size
    returns y: output sound
    """

    # half analysis window size by rounding
    hM1 = (M+1)//2
    # half analysis window size by floor
    hM2 = M//2
    # number of frames
    nFrames = mY[:,0].size
    # initialize output array
    y = np.zeros(nFrames*H + hM1 + hM2)
    pin = hM1
    # iterate over all frames
    for i in range(nFrames):
        # compute idft
        y1 = speech_model_dft.dftSynth(mY[i,:], pY[i,:], M)
        # overlap-add to generate output sound
        y[pin-hM1:pin+hM2] += H*y1
        # advance sound pointer
        pin += H

    # delete half of first window which was added in stftAnal
    y = np.delete(y, range(hM2))
    # delete the end of the sound that was added in stftAnal
    y = np.delete(y, range(y.size-hM1, y.size))
    return y
