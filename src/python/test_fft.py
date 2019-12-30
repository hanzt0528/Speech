import numpy as np 
import matplotlib.pyplot as plt
from scipy.fftpack import fft,fftshift,ifft
from scipy.signal import sawtooth
import speech_fft as speech_fft


def test_fft():
    x = np.array([0,1,2,3,4,5,6,7])
    X = speech_fft.fft(x,8)
    print(X)
    X2 = fft(x)
    print(X2)

test_fft()
