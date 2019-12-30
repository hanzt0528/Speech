import pdb
import numpy as np
import matplotlib.pyplot as plt
from scipy.fftpack import fft,fftshift,ifft
from scipy.signal import sawtooth
from scipy.signal import triang, resample,blackmanharris
import math
from scipy import signal
import math, copy, sys, os
from scipy.io.wavfile import write, read
from sys import platform
import subprocess
from scipy.signal import get_window

from ispeech.source import *


sine = SineGenerator(44100)
sine.setFrequency(440.0)
sine.setAmplitude(2**15-1)
sine.setPhase(1.0)
sine.generate(4410)

sine2 = SineGenerator(44100)
sine2.setFrequency(440.0*2)
sine2.setAmplitude(2**15-1)
sine2.setPhase(1.0)
sine2.generate(4410)


s1 = np.array(sine);
s2 = np.array(sine2);

s = s1+s2
plt.plot(np.arange(s.size),s)
plt.show()
