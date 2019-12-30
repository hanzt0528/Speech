import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *
from ispeech.transform import *
from scipy.io.wavfile import write, read
import math, copy, sys, os

def wavread(filename):
    """
    Read a sound file and convert it to a normalized floating point array
    filename: name of file to read
    returns fs: sampling rate of file, x: floating point array
    """

    if (os.path.isfile(filename) == False):                  # raise error if wrong input file
            raise ValueError("Input file is wrong")

    fs, x = read(filename)

    if (len(x.shape) !=1):                                   # raise error if more than one channel
            raise ValueError("Audio file should be mono")

    if (fs !=44100):                                         # raise error if more than one channel
            raise ValueError("Sampling rate of input sound should be 44100")

    INT16_FAC = (2**15)-1
    INT32_FAC = (2**31)-1
    INT64_FAC = (2**63)-1
    norm_fact = {'int16':INT16_FAC, 'int32':INT32_FAC, 'int64':INT64_FAC,'float32':1.0,'float64':1.0}

    #scale down and convert audio into floating point number in range of -1 to 1
    x = np.float32(x)/norm_fact[x.dtype.name]
    return fs, x

        
def Autocorrelation(signal):
    a = np.zeros(signal.size)
    for k in np.arange(signal.size):
        for i in np.arange(signal.size-k):
            a[k] += signal[i]*signal[i+k]

    return a

def DetectPitch(a,fs):
    kmax = 0
    for k in np.arange(a.size):
        if a[k]< 0:
            new_a = a[k:]
            new_a = new_a.tolist()
            kmax = new_a.index(max(new_a))+k
            break

    kmax = peakInterp(a,kmax)
    return kmax,fs/kmax

def peakInterp(mX,ploc):
    """
    Interpolate peak values using parabolic interpolation
    mX, pX: magnitude and phase spectrum, ploc: locations of peaks
    returns iploc, ipmag, ipphase: interpolated peak location, magnitude and phase values
    """

    # magnitude of peak bin
    val = mX[ploc]
    # magnitude of bin at left
    lval = mX[ploc-1]
    # magnitude of bin at right
    rval = mX[ploc+1]
    # center of parabola
    iploc = ploc + 0.5*(lval-rval)/(lval-2*val+rval)

    return iploc

def frequency2note(f):
    p = 69+12*np.log2(f/440.0)

    return p

def note2frequency(note):
    f = 440*np.power(2,(p-69)/12)

    return f


def test_Autocorrelation():

    
    signal = [1,1,1,1,-1,-1,-1,-1,1,1,1,1,-1,-1,-1,-1];

    signal = np.array(signal)

    fs = 11025;
    t =   np.arange(0,0.2,1.0/fs)
    
    signal = np.cos(2*np.pi*440*t)

    #fs,signal= wavread("./A4-440Hz-69.wav")
    fs,signal= wavread("./twinkle.wav")

    print("size of signal = " + str(signal.size))
    signal=signal[int(4*fs):int(6*fs)]
    plt.figure(1, figsize=(9.5, 6))

    plt.subplot(2,1,1)
    plt.plot(np.arange(signal.size),signal,label="signal")

    plt.title("signal = np.cos(2*np.pi*100*t)+np.cos(2*np.pi*200*t)+np.cos(2*np.pi*400*t)")
    a = Autocorrelation(signal)

    kmax,f = DetectPitch(a,fs)

    print('kmax = ' + str(kmax))
    print('f = '+ str(fs/kmax))
    plt.subplot(2,1,2)

    plt.plot(np.arange(a.size),a,label="when t=0.2s " + "kmax =" + str(kmax)+ " f="+ str(f));

    note = frequency2note(f)

    print('note = ' + str(note))
    plt.title("Autocorrelation of signal")
    plt.legend()
    plt.show()
    
   
test_Autocorrelation()
