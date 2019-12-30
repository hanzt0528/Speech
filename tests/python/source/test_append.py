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

sine2 = SineGenerator(fs)
sine2.setFrequency(frequency/2)
sine2.setAmplitude(2**15-1)
sine2.setPhase(1.0)
sine2.generate(int(fs/2))

sine.appendHead(sine2)

s = np.array(sine);

plt.plot(np.arange(len(sine2)),s[:len(sine2)],'g',label='append')
plt.plot(np.arange(len(sine2),s.size,1),s[len(sine2):],'b',label='original signal')

plt.legend()
plt.title('test append')
plt.show()
