import pdb
import numpy as np 
import matplotlib.pyplot as plt
from scipy.signal import get_window
import math
import speech_model_sine as speech_model_sine
import speech_source as speech_source
import speech_model_stft as speech_model_stft
from ispeech.source import *
from ispeech.transform import *

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

def test_sineAnal():
    
    (fs, x) = speech_source.wavread('../../data/flute-A4.wav')

    fs = 44100#sample rate
    f = 500#frequency of signal
    M = 1024#signal size
    sine = SineGenerator(fs)
    sine.setFrequency(f)
    sine.setAmplitude(1.0)
    sine.setPhase(1.0)
    sine.generate(M)

    dftModel = ModelDFT()
    hamming = HammingWindow(M)

    
    w = np.blackman(601)
    N = 1024
    H = 150
    t = -80
    minSineDur = .01
    maxnSines = 150

    ms = ModelSine()

    x = np.array(sine)
    w = np.array(hamming)
    
    mX, pX = speech_model_stft.stftAnal(x, w, N, H)

    tfreq, tmag, tphase = speech_model_sine.sineModelAnal(x, fs, w, N, H, t, maxnSines, minSineDur,20.0,0.01)

    
    plt.figure(1, figsize=(9.5, 5))
    plt.subplot(3,1,1)
    maxplotfreq = 5000.0
    maxplotbin = int(N*maxplotfreq/fs)
    numFrames = int(mX[:,0].size)
    frmTime = H*np.arange(numFrames)/float(fs)                             
    binFreq = np.arange(maxplotbin+1)*float(fs)/N                         
    plt.pcolormesh(frmTime, binFreq, np.transpose(mX[:,:maxplotbin+1]))
    plt.autoscale(tight=True)

    print('tfreq =')
    print(tfreq)
    
    tracks = tfreq*np.less(tfreq, maxplotfreq)
    tracks[tracks<=0] = np.nan
    print('tracks size = '+str(tracks.size))
    print('tracks = ')
    print(tracks)
    
    plt.plot(frmTime, tracks, color='k', lw=1.5)
    plt.autoscale(tight=True)
    plt.title('mX + sinusoidal tracks (flute-A4.wav)')

    plt.subplot(3,1,2)

    Ns = 512
    #H = Ns//4
    
    y = speech_model_sine.sineModelSynth(tfreq,tmag,tphase,Ns,H,fs)
    print(numFrames*H)
    plt.plot(np.arange(y.size),y)
    plt.tight_layout()
    #plt.savefig('sineModelAnal-flute.png')
    plt.show()
    
#test_Interp()
#test_sineModel()
test_sineAnal()
