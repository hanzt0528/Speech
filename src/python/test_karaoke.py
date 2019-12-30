import pdb
import numpy as np 
import matplotlib.pyplot as plt
from scipy.signal import get_window
import math

import essentia.standard as ess
energy = ess.Energy()
rms = ess.RMS()
loudness = ess.Loudness()

import speech_model_sine as speech_model_sine
import speech_source as speech_source
import speech_model_stft as speech_model_stft

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

    if ploc+1 >= mX.size:
        return ploc
    
    rval = mX[ploc+1]
    # center of parabola
    iploc = ploc + 0.5*(lval-rval)/(lval-2*val+rval)

    return iploc

def test_Interp():
    xp=[1,2,3]
    fp=[3,2,0]
    x=[0,1,1.57,2.5,3.14]
    ix=np.interp(x,xp,fp)
    ix2=speech_model_sine.Interp(x,xp,fp)
    print(ix)
    print(ix2)

def test_sineModel():
    fs = 44100#sample rate
    f = 500#frequency of signal
    M = 256#signal size
    N = 512#FFT size
    L = 256*3
    
    sine = speech_source.gensine(L,f,fs)    
    hamming = get_window('hamming',M)
    y  = speech_model_sine.sineModel(sine,fs,hamming,N,-100.0)

    #show windowed signal by hamming window
    x = np.array(sine)
    plt.figure(1)
    plt.subplot(2,1,1)
    plt.plot(np.arange(x.size),x)
    plt.title('windowed sine signal')


    #show mode synthesis signal
    plt.subplot(2,1,2)
    plt.plot(np.arange(len(y)),y,'g')
    plt.title('use model synthesis signal')

    plt.show()


_spectrumMemory = np.array([])
def spectral_flux(spectrum):

  global _spectrumMemory
  if _spectrumMemory.size == 0 :
    _spectrumMemory = np.zeros(spectrum.size)

  flux = 0.0
  for i in range(0,int(spectrum.size)):
    flux += (spectrum[i]-_spectrumMemory[i])*(spectrum[i]-_spectrumMemory[i])

  flux = np.sqrt(flux)

  _spectrumMemory = np.copy(spectrum)
  
  return flux
  
  
#Onset detection based on high-frequency content
def speech_hfc(spectrum,fs):
  # Coefficient to convert bins into frequency

  bin2hz = 0.
  
  if spectrum.size > 1 :
    bin2hz = (fs/2) /(spectrum.size - 1)


  hfc = 0.0
  
  for i in range(0,int(spectrum.size)):
    #Masri
    #hfc+=mX[i]*frame[i]*bin2hz*i
    #Jensen
    #hfc+=mX[i]*bin2hz*i*bin2hz*i
    #Brossier
   
    hfc+=spectrum[i]*bin2hz*i
    
  return hfc


def speech_energy(frame):
    energy = 0.0
    for i in frame:
        energy+=i*i
    return energy

def speech_rms(frame):
    rms =np.sqrt(speech_energy(frame))/frame.size
    return rms

def speech_loudness(frame):
    loudness =pow(speech_energy(frame),0.67)
    return loudness

def speech_loudness_frames2(frames,hop_size):
    pin = 0;
    pend = frames.size- hop_size
    rmses = np.array([])
    while pin < pend:
        frame = frames[pin:pin+hop_size]
        r = loudness(frame)
        rmses = np.append(rmses,r)
        pin+=hop_size

    return rmses

def speech_loudness_frames(frames,hop_size):
    pin = 0;
    pend = frames.size- hop_size
    rmses = np.array([])
    while pin < pend:
        frame = frames[pin:pin+hop_size]
        r = speech_loudness(frame)
        rmses = np.append(rmses,r)
        pin+=hop_size

    return rmses

def speech_rms_frames2(frames,hop_size):
    pin = 0;
    pend = frames.size- hop_size
    rmses = np.array([])
    while pin < pend:
        frame = frames[pin:pin+hop_size]
        r = rms(frame)
        rmses = np.append(rmses,r)
        pin+=hop_size

    return rmses

def speech_rms_frames(frames,hop_size):
    pin = 0;
    pend = frames.size- hop_size
    rmses = np.array([])
    while pin < pend:
        frame = frames[pin:pin+hop_size]
        r = speech_rms(frame)
        rmses = np.append(rmses,r)
        pin+=hop_size

    return rmses

def speech_energy_frames2(frames,hop_size):
    pin = 0;
    pend = frames.size- hop_size
    energies = np.array([])
    while pin < pend:
        frame = frames[pin:pin+hop_size]
        egy = energy(frame)
        energies = np.append(energies,egy)
        pin+=hop_size

    return energies

def speech_energy_frames(frames,hop_size):
    pin = 0;
    pend = frames.size- hop_size
    energies = np.array([])
    while pin < pend:
        frame = frames[pin:pin+hop_size]
        egy = speech_energy(frame)
        energies = np.append(energies,egy)
        pin+=hop_size

    return energies
    
def test_sineAnal():

    wav = '../../data/MIR-1K/Wavfile/mono/abjones_1_01.wav'
    #wav = '../../data/midi/twinkle.wav'
    (fs, x) = speech_source.wavread(wav)

    N = 1024
    w = np.hanning(N)

    H = 512
    t = -80
    minSineDur = .1
    maxnSines = 150

    x=x[40100:44100+44100+ 22050]
    #x=x[40100:44100 + 6000]

    #x=x
    plt.figure(1, figsize=(9.5, 5))

    plt.subplot(4,1,1)
    
    plt.plot(np.arange(x.size)/fs,x)

  
    energies = speech_energy_frames(x,H)

    energies = energies/max(energies);

    plt.plot(H*np.arange(energies.size)/float(fs),energies,label = 'energy me')


   
    mX, pX = speech_model_stft.stftAnal(x, w, N, H)

    maxplotfreq = 5000.0
    maxplotbin = int(N*maxplotfreq/fs)
    numFrames = int(mX[:,0].size)
    frmTime = H*np.arange(numFrames)/float(fs)                             
    binFreq = np.arange(maxplotbin+1)*float(fs)/N                         

    hfcs = []
    fluxes= []
    for i in range(0,numFrames):
        magnitude= mX[i]
        spectrum = pow(10,magnitude/20.0)
        flux = spectral_flux(spectrum)
        hfc = speech_hfc(spectrum,fs)
        fluxes.append(flux)
        hfcs.append(hfc)

    hfcs = np.array(hfcs)
    hfcs = np.array(hfcs)

    plt.plot(frmTime,  fluxes /max(fluxes) ,label = 'spectral flux',lw=1.5,marker = 'x')
    plt.plot(frmTime,  hfcs /max(hfcs) ,label = 'hfc',lw=1.5,marker = 'x')
    plt.legend()

    plt.subplot(4,1,2)
    plt.pcolormesh(frmTime, binFreq, np.transpose(mX[:,:maxplotbin+1]))
    plt.autoscale(tight=True)


    plt.tight_layout()

    plt.subplot(4,1,3)

    i = int(numFrames/2)
    plt.plot(np.arange(mX[i].size),mX[i])


    # get pitch

    pin = 0
    pend = x.size - H

    print('x.size ='+str(x.size))

    pitches = []
    while pin < pend :
        x1 = x[pin:pin+H]
        
        a = Autocorrelation(x1)

        kmax,f = DetectPitch(a,fs)

        f = 69+np.log2(f/440.0)
        pitches.append(f)
        pin+=H


    pitches = np.array(pitches)

    print('pitch size = ' + str(pitches.size))

    plt.subplot(4,1,4)
    plt.plot(np.arange(pitches.size)*H/float(fs),pitches,'g',marker = 'x',label = 'note')

    plt.legend()
    plt.tight_layout()
    #plt.savefig('sineModelAnal-flute.png')
    plt.show()

#test_Interp()
#test_sineModel()
test_sineAnal()
