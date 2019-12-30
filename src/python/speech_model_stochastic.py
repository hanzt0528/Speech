import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import resample
from scipy.fftpack import fft, ifft

import speech_source as speech_source

# import speech_model_dft as speech_model_dft
# import speech_model_sine as speech_model_sine
# import speech_model_stft as speech_model_stft

def stochasticModelFrame(x, w, N, stocf):

    # x: input array sound, w: analysis window, N: FFT size,
    # stocf: decimation factor of mag spectrum for stochastic analysis
    # size of positive spectrum
    hN = N//2+1
    # half analysis window size
    hM = (w.size)//2
    # initialize sound pointer in middle of analysis window
    pin = hM
    # initialize buffer for FFT
    fftbuffer = np.zeros(N)
    # initialize output sound frame
    # yw = np.zeros(w.size)
    # normalize analysis window
    w = w / sum(w)
    # -----analysis-----
    # window the input sound
    xw = x[pin-hM:pin+hM] * w
    # compute FFT
    X = fft(xw)
    # magnitude spectrum of positive frequencies
    mX = 20 * np.log10(abs(X[:hN]))
    # decimate the mag spectrum
    mXenv = resample(np.maximum(-200, mX), int(mX.size*stocf))
    pX = np.angle(X[:hN])
    # -----synthesis-----
    # interpolate to original size
    mY = resample(mXenv, hN)
    # generate phase random values
    pY = 2*np.pi*np.random.rand(hN)
    Y = np.zeros(N, dtype=complex)
    # generate positive freq.
    Y[:hN] = 10**(mY/20) * np.exp(1j*pY)
    # generate negative freq.
    Y[hN:] = 10**(mY[-2:0:-1]/20) * np.exp(-1j*pY[-2:0:-1])
    # inverse FFT
    fftbuffer = np.real(ifft(Y))
    y = fftbuffer*N/2
    return mX, pX, mY, pY, y

# for a frame
def test_stochasticModelFrame():
    (fs, x) = speech_source.wavread('../../../Speech/data/ocean.wav')
    w = np.hanning(1024)
    N = 1024
    stocf = 0.2
    maxFreq = 10000.0
    # lastbin = int(N*maxFreq/fs)
    first = 1000
    last = first+w.size
    mX, pX, mY, pY, y = stochasticModelFrame(x[first:last], w, N, stocf)

    plt.figure(1, figsize=(9, 7))
    plt.subplot(4, 1, 1)
    plt.plot(np.arange(first, last)/float(fs), x[first:last])
    plt.axis([first/float(fs), last/float(fs),
              min(x[first:last]), max(x[first:last])])

    plt.title('x (ocean.wav)')
    plt.subplot(4, 1, 2)
    plt.plot(float(fs)*np.arange(mX.size)/N, mX, 'r', lw=1.5, label="mX")
    plt.plot(float(fs)*np.arange(mY.size)/N, mY, 'k', lw=1.5, label="mY")
    plt.legend()
    plt.axis([0, maxFreq, -80, max(mX)+3])
    plt.title('mX + mY (stochastic approximation)')
    plt.subplot(4, 1, 3)
    plt.plot(float(fs)*np.arange(pX.size)/N, pX, 'c', lw=1.5, label="pX")
    plt.plot(float(fs)*np.arange(pY.size)/N, pY-np.pi, 'k', lw=1.5, label="pY")
    plt.axis([0, maxFreq, -np.pi, np.pi])
    plt.legend()
    plt.title('pX + pY (random phases)')
    plt.subplot(4, 1, 4)
    plt.plot(np.arange(first, last)/float(fs), y, 'b', lw=1.5)
    plt.axis([first/float(fs), last/float(fs), min(y), max(y)])
    plt.title('y')

    plt.tight_layout()
    # plt.savefig('stochasticModelFrame.png')
    plt.show()



test_stochasticModelFrame()
