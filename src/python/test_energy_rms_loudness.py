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
def spectral_flux(mX):
  global _spectrumMemory
  if _spectrumMemory.size == 0 :
    _spectrumMemory = np.zeros(mX.size)

  flux = 0.0
  for i in range(0,int(mX.size)):
    flux += (mX[i]-_spectrumMemory[i])*(mX[i]-_spectrumMemory[i])

  flux = np.sqrt(flux)

  _spectrumMemory = np.copy(mX)
  
  return flux
  
  
#Onset detection based on high-frequency content
def speech_hfc(mX,fs):
  # Coefficient to convert bins into frequency
  bin2hz = 0.

  
  if mX.size > 1 :
    bin2hz = (fs/2) /(mX.size - 1)

  hfc = 0.0
  
  for i in range(0,int(mX.size)):
    #Masri
    #hfc+=frame[i]*frame[i]*bin2hz*i
    #Jensen
    #hfc+=frame[i]*bin2hz*i*bin2hz*i
    #Brossier
    hfc+=mX[i]*bin2hz*i
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
    
    (fs, x) = speech_source.wavread('../../data/MIR-1K/Wavfile/mono/abjones_1_01.wav')
    w = np.blackman(601)
    N = 1024
    H = 150
    t = -80
    minSineDur = .1
    maxnSines = 150

    x=x[40100:44100+44100+ 22050]

    #x=x
    plt.figure(1, figsize=(9.5, 5))

    plt.subplot(3,1,1)
    
    plt.plot(np.arange(x.size)/fs,x)

  
    energies = speech_energy_frames(x,H)

    energies = energies/max(energies);

    energies2 = speech_energy_frames2(x,H)

    energies2 = energies2/max(energies2);

    plt.plot(H*np.arange(energies.size)/float(fs),energies,label = 'energy me')

    plt.plot(H*np.arange(energies2.size)/float(fs),energies2,label = 'energy essentia')

    
    energies = speech_rms_frames(x,H)

    energies = energies/max(energies);

    energies2 = speech_rms_frames2(x,H)

    energies2 = energies2/max(energies2);

    plt.plot(H*np.arange(energies.size)/float(fs),energies,label="rms me")

    plt.plot(H*np.arange(energies2.size)/float(fs),energies2,label = "rms essentia")

    energies = speech_loudness_frames(x,H)

    energies = energies/max(energies);

    energies2 = speech_loudness_frames2(x,H)

    energies2 = energies2/max(energies2);

    plt.plot(H*np.arange(energies.size)/float(fs),energies,label="loudness me")

    plt.plot(H*np.arange(energies2.size)/float(fs),energies2,label = "loudness essentia")


    plt.legend()
    
    plt.show()
    return
    plt.subplot(3,1,2)
    mX, pX = speech_model_stft.stftAnal(x, w, N, H)

    maxplotfreq = 5000.0
    maxplotbin = int(N*maxplotfreq/fs)
    numFrames = int(mX[:,0].size)
    frmTime = H*np.arange(numFrames)/float(fs)                             
    binFreq = np.arange(maxplotbin+1)*float(fs)/N                         
    plt.pcolormesh(frmTime, binFreq, np.transpose(mX[:,:maxplotbin+1]))
    plt.autoscale(tight=True)


    plt.tight_layout()
    #plt.savefig('sineModelAnal-flute.png')
    plt.show()

#test_Interp()
#test_sineModel()
test_sineAnal()
