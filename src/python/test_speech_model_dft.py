import numpy as np 
import matplotlib.pyplot as plt
from scipy.fftpack import fft,fftshift,ifft
from scipy.signal import sawtooth
import speech_model_dft as speech_model_dft
from scipy.signal import get_window
import math
def test_model_dft():
    #gen sine wave input signal 
    t = 6
    fs = 100.0
    f = 5.0
    nT = np.arange(0,t,1/fs);
    phi = 0
    A = 1
    nT = nT[:512]
    x=A*np.sin(2*np.pi*f*nT+phi)

    plt.figure(1)
    plt.subplot(5,1,1)
    plt.plot(nT,x,label="sinewave")
    plt.legend()

    #draw spectrum wave
    N = x.size
    m = np.arange(0,N,1)
    dt = 1/fs
    tm=m*dt;

    #fft
    X = np.array([])
    for k in range(N):
        s = np.exp( -1j*2*np.pi*k*tm )
        X=np.append(X,sum(x*s))

    plt.subplot(5,1,2)
    plt.plot(np.arange(X.size),abs(X),label="DFT:magnitude spectrum")
    plt.legend()
    #draw inverse sinewave
    #ifft
    xs = np.array([])
    for n in range(N):
        k=n
        s = np.exp( -1j*2*np.pi*k*tm )/t
        xs = np.append(xs,sum(X*np.conjugate(s)))

    xs/=N

    plt.subplot(5,1,3)
    plt.plot(nT,np.real(xs),label="IDFT")
    plt.legend()

    hamming = get_window('hamming',N)

    
    mX,pX = speech_model_dft.dftAnal(x,hamming,N)

    
    plt.subplot(5,1,4)
    plt.plot(np.arange(mX.size),mX,'g',label="DFT:magnitude spectrum")

    y=speech_model_dft.dftSynth(mX,pX,N)
    
 
    plt.subplot(5,1,5)
    plt.plot(np.arange(y.size),abs(y),'g',label="DFT:magnitude spectrum")   
    plt.show()


test_model_dft()
