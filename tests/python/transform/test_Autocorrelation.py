import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *
from ispeech.transform import *

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

def test_Autocorrelation():

    
    signal = [1,1,1,1,-1,-1,-1,-1,1,1,1,1,-1,-1,-1,-1];

    signal = np.array(signal)

    fs = 11025;
    t =   np.arange(0,0.2,1.0/fs)
    
    signal = np.cos(2*np.pi*100*t)+np.cos(2*np.pi*200*t)+np.cos(2*np.pi*400*t)

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

    t =   np.arange(0,0.06,1.0/fs)
    
    signal = np.cos(2*np.pi*100*t)+np.cos(2*np.pi*200*t)+np.cos(2*np.pi*400*t)

    a = Autocorrelation(signal)

    kmax,f = DetectPitch(a,fs)

    print('kmax = ' + str(kmax))
    print('f = '+ str(fs/kmax))

    plt.plot(np.arange(a.size),a,label="when t=0.02s " + "kmax =" + str(kmax)+ " f="+ str(f));

    plt.title("Autocorrelation of signal")
    plt.legend()
    plt.show()
    
   
test_Autocorrelation()
