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

fs = 44100
#frequency means how much circle each 1 minitue
frequency = 20
sine = SineGenerator(fs)
sine.setFrequency(frequency)
sine.setAmplitude(2**15-1)
sine.setPhase(1.0)
sine.generate(int(fs))


s = np.array(sine);

plt.plot(np.arange(s.size),s)
plt.show()


