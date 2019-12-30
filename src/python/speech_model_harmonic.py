import numpy as np
import math
import matplotlib.pyplot as plt
from scipy.signal import triang, resample,blackmanharris
from scipy.fftpack import fft,fftshift,ifft

import speech_model_dft as speech_model_dft
import speech_model_sine as speech_model_sine
import speech_source as speech_source
import speech_model_stft as speech_model_stft
def TWM_MY (pfreq, pmag, maxnpeaks, f0c):
  # Two-way mismatch algorithm for f0 detection (by Beauchamp&Maher)
  # pfreq, pmag: peak frequencies in Hz and magnitudes, maxnpeaks: maximum number of peaks used
  # f0cand: frequencies of f0 candidates
  # returns f0: fundamental frequency detected

  p = 0.5                                          # weighting by frequency value
  q = 1.4                                          # weighting related to magnitude of peaks
  r = 0.5                                          # scaling related to magnitude of peaks
  rho = 0.33                                       # weighting of MP error
  Amax = max(pmag)                                 # maximum peak magnitude
  harmonic = np.matrix(f0c)
  ErrorPM = np.zeros(harmonic.size)                 # initialize PM errors
  MaxNPM = min(maxnpeaks, pfreq.size)
  #print(MaxNPM)

  #calculate predicted to measured error.
  frequencies=pfreq
  mags=np.array(pmag)
  Amax = max(mags)
  #ew for each predicted fundamental frequency
  error_p_to_m = np.zeros(f0c.size)
  #calculate each one in f0c,then get the minimum one for f0 at l .

  for f in range(0,f0c.size):
    ffund = f0c[f]
    fn = np.zeros(MaxNPM)
    deltafn = np.zeros(MaxNPM)
    #make all of fn

    maxN=maxnpeaks
    fn = np.zeros(maxN)
    deltafn = np.zeros(maxN)

    for n in range(0,maxN):
      fn[n] = ffund + n*ffund

      #get deltafn and an at n.
      deltafn[n] = abs(fn[n]-frequencies[0])
      an = mags[0]
      for i in range(0,frequencies.size):
        fk = frequencies[i]
        if(abs(fn[n]-fk)< deltafn[n]):
          deltafn[n] = abs(fn[n]-fk)
          an = mags[i]
      #caculate the error of predicted to measured one at n .
      error_p_to_m[f]+=deltafn[n]*(fn[n]**(-p))+ (10**((an-Amax)/20))*(q*deltafn[n]*(fn[n]**(-p))-r)

  #caculate measured to predicted error.
  MaxNMP =  min(maxnpeaks, pfreq.size)
  error_m_to_p = np.zeros(f0c.size)
  for f in range(0,f0c.size):
    ffund = f0c[f]
    maxN = int(np.ceil(frequencies[MaxNMP-1]/ffund))
    fn = np.zeros(maxN)
    for n in range(1,maxN+1):
      fn[n-1] = n*ffund

    deltafk = np.zeros(MaxNMP)
    fk = np.zeros(MaxNMP)
    #for k in range(0,frequencies.size):

    for k in range(0,MaxNMP):
      fk[k] = frequencies[k]
      deltafk[k]= abs(fn[0]-fk[k])
      ak = mags[k]

      for n in range(0,maxN):
        if( abs(fn[n]-fk[k]) < deltafk[k] ):
          deltafk[k] = abs(fn[n]-fk[k])
          ak = mags[k-1]
      MagFactor = 10**((ak-Amax)/20)
      error_m_to_p[f]+=MagFactor*( deltafk[k]*(fk[k]**(-p))+MagFactor*(q*deltafk[k]*(fk[k]**(-p))-r) )

  Error = (error_p_to_m/MaxNPM) + (rho*error_m_to_p/MaxNMP)
  f0index = np.argmin(Error)                       # get the smallest error
  f0 = f0c[f0index]
  f0error = Error[f0index]
  return f0,f0error,error_p_to_m,error_m_to_p,Error

def Twm (pfreq, pmag,f0c):

    maxnpeaks = 10
    f0,f0error,ErrorPM,ErrorMP,Error = TWM_MY (pfreq, pmag, maxnpeaks, f0c)

    return f0,f0error

def TWM_p(pfreq, pmag, f0c):
      """
      Two-way mismatch algorithm for f0 detection (by Beauchamp&Maher)
      [better to use the C version of this function: UF_C.twm]
      pfreq, pmag: peak frequencies in Hz and magnitudes,
      f0c: frequencies of f0 candidates
      returns f0, f0Error: fundamental frequency detected and its error
      """

      p = 0.5                                          # weighting by frequency value
      q = 1.4                                          # weighting related to magnitude of peaks
      r = 0.5                                          # scaling related to magnitude of peaks
      rho = 0.33                                       # weighting of MP error
      Amax = max(pmag)                                 # maximum peak magnitude
      maxnpeaks = 10                                   # maximum number of peaks used
      harmonic = np.matrix(f0c)
      ErrorPM = np.zeros(harmonic.size)                # initialize PM errors
      MaxNPM = min(maxnpeaks, pfreq.size)
      for i in range(0, MaxNPM) :                      # predicted to measured mismatch error
              difmatrixPM = harmonic.T * np.ones(pfreq.size)
              difmatrixPM = abs(difmatrixPM - np.ones((harmonic.size, 1))*pfreq)
              FreqDistance = np.amin(difmatrixPM, axis=1)    # minimum along rows
              peakloc = np.argmin(difmatrixPM, axis=1)
              Ponddif = np.array(FreqDistance) * (np.array(harmonic.T)**(-p))
              PeakMag = pmag[peakloc]
              MagFactor = 10**((PeakMag-Amax)/20)
              ErrorPM = ErrorPM + (Ponddif + MagFactor*(q*Ponddif-r)).T
              harmonic = harmonic+f0c

      ErrorMP = np.zeros(harmonic.size)                # initialize MP errors
      MaxNMP = min(maxnpeaks, pfreq.size)
      for i in range(0, f0c.size) :                    # measured to predicted mismatch error
              nharm = np.round(pfreq[:MaxNMP]/f0c[i])
              nharm = (nharm>=1)*nharm + (nharm<1)
              FreqDistance = abs(pfreq[:MaxNMP] - nharm*f0c[i])
              Ponddif = FreqDistance * (pfreq[:MaxNMP]**(-p))
              PeakMag = pmag[:MaxNMP]
              MagFactor = 10**((PeakMag-Amax)/20)
              ErrorMP[i] = sum(MagFactor * (Ponddif + MagFactor*(q*Ponddif-r)))

      Error = (ErrorPM[0]/MaxNPM) + (rho*ErrorMP/MaxNMP)  # total error
      f0index = np.argmin(Error)                       # get the smallest error
      f0 = f0c[f0index]                                # f0 with the smallest error

      return f0, Error[f0index]


def f0Twm(pfreq, pmag, ef0max, minf0, maxf0, f0t=0):
    """
    Function that wraps the f0 detection function TWM, selecting the possible f0 candidates
    and calling the function TWM with them
    pfreq, pmag: peak frequencies and magnitudes,
    ef0max: maximum error allowed, minf0, maxf0: minimum  and maximum f0
    f0t: f0 of previous frame if stable
    returns f0: fundamental frequency in Hz
    """
    # raise exception if minf0 is smaller than 0
    if (minf0 < 0):
        raise ValueError("Minimum fundamental frequency (minf0) smaller than 0")

    # raise exception if maxf0 is bigger than 10000Hz
    if (maxf0 >= 10000):
        raise ValueError("Maximum fundamental frequency (maxf0) bigger than 10000Hz")

    # return 0 if less than 3 peaks and not previous f0
    if (pfreq.size < 3) & (f0t == 0):
        return 0

    # use only peaks within given range
    f0c = np.argwhere((pfreq>minf0) & (pfreq<maxf0))[:,0]
    # return 0 if no peaks within range
    if (f0c.size == 0):
        return 0
    # frequencies of peak candidates
    f0cf = pfreq[f0c]
    # magnitude of peak candidates
    f0cm = pmag[f0c]

    # if stable f0 in previous frame
    if f0t>0:
        # use only peaks close to it
        shortlist = np.argwhere(np.abs(f0cf-f0t)<f0t/2.0)[:,0]
        maxc = np.argmax(f0cm)
        maxcfd = f0cf[maxc]%f0t
        if maxcfd > f0t/2:
            maxcfd = f0t - maxcfd
        # or the maximum magnitude peak is not a harmonic
        if (maxc not in shortlist) and (maxcfd>(f0t/4)):
            shortlist = np.append(maxc, shortlist)
        # frequencies of candidates
        f0cf = f0cf[shortlist]

    # return 0 if no peak candidates
    if (f0cf.size == 0):
        return 0

    # call the TWM function with peak candidates
    f0, f0error = Twm(pfreq, pmag, f0cf)
    #f0, f0error = TWM_p(pfreq, pmag, f0cf)
    #print("f0,f0error="+str(f0)+","+str(f0error))

    # accept and return f0 if below max error allowed
    if (f0>0) and (f0error<ef0max):
        return f0
    else:
        return 0

def f0Detection(x, fs, w, N, H, t, minf0, maxf0, f0et):
    """
    Fundamental frequency detection of a sound using twm algorithm
    x: input sound; fs: sampling rate; w: analysis window;
    N: FFT size; t: threshold in negative dB,
    minf0: minimum f0 frequency in Hz, maxf0: maximim f0 frequency in Hz,
    f0et: error threshold in the f0 detection (ex: 5),
    returns f0: fundamental frequency
    """

    # raise exception if minf0 is smaller than 0
    if (minf0 < 0):
        raise ValueError("Minumum fundamental frequency (minf0) smaller than 0")

    # raise exception if maxf0 is bigger than fs/2
    if (maxf0 >= 10000):
        raise ValueError("Maximum fundamental frequency (maxf0) bigger than 10000Hz")

    # raise error if hop size 0 or negative
    if (H <= 0):
        raise ValueError("Hop size (H) smaller or equal to 0")

    # size of positive spectrum
    hN = N//2
    # half analysis window size by rounding
    hM1 = int(math.floor((w.size+1)/2))
    # half analysis window size by floor
    hM2 = int(math.floor(w.size/2))
    # add zeros at beginning to center first window at sample 0
    x = np.append(np.zeros(hM2),x)
    # add zeros at the end to analyze last sample
    x = np.append(x,np.zeros(hM1))
    # init sound pointer in middle of anal window
    pin = hM1
    # last sample to start a frame
    pend = x.size - hM1
    # initialize buffer for FFT
    fftbuffer = np.zeros(N)
    # normalize analysis window
    w = w / sum(w)
    # initialize f0 output
    f0 = []
    # initialize f0 track
    f0t = 0
    # initialize f0 stable
    f0stable = 0
    while pin<pend:
        # select frame
        x1 = x[pin-hM1:pin+hM2]
        # compute dft
        mX, pX = speech_model_dft.dftAnal(x1, w, N)
        # detect peak locations
        ploc = speech_model_sine.peakDetection(mX, t)
        # refine peak values
        iploc, ipmag, ipphase = speech_model_sine.peakInterp(mX, pX, ploc)
        # convert locations to Hez
        ipfreq = fs * iploc/N
        # find f0
        f0t = f0Twm(ipfreq, ipmag, f0et, minf0, maxf0, f0stable)

        if ((f0stable==0)&(f0t>0)) \
                        or ((f0stable>0)&(np.abs(f0stable-f0t)<f0stable/5.0)):
            # consider a stable f0 if it is close to the previous one
            f0stable = f0t
        else:
            f0stable = 0
        # add f0 to output array
        f0 = np.append(f0, f0t)
        # advance sound pointer
        pin += H
    return f0
def harmonicDetection(pfreq, pmag, pphase, f0, nH, hfreqp, fs, harmDevSlope=0.01):
  """
  Detection of the harmonics of a frame from a set of spectral peaks using f0
  to the ideal harmonic series built on top of a fundamental frequency
  pfreq, pmag, pphase: peak frequencies, magnitudes and phases
  f0: fundamental frequency,
  nH: number of harmonics,
  hfreqp: harmonic frequencies of previous frame,
  fs: sampling rate;
  harmDevSlope: slope of change of the deviation allowed to perfect harmonic
  returns hfreq, hmag, hphase: harmonic frequencies, magnitudes, phases
  """

  # if no f0 return no harmonics
  if (f0<=0):
    return np.zeros(nH), np.zeros(nH), np.zeros(nH)
  # initialize harmonic frequencies
  hfreq = np.zeros(nH)
  # initialize harmonic magnitudes
  hmag = np.zeros(nH)-100
  # initialize harmonic phases
  hphase = np.zeros(nH)
  # initialize harmonic frequencies
  hf = f0*np.arange(1, nH+1)
  # initialize harmonic index
  hi = 0
  # if no incomming harmonic tracks initialize to harmonic series
  if hfreqp == []:
    hfreqp = hf
  # find harmonic peaks
  while (f0>0) and (hi<nH) and (hf[hi]<fs/2):
    # closest peak
    pei = np.argmin(abs(pfreq - hf[hi]))
    # deviation from perfect harmonic
    dev1 = abs(pfreq[pei] - hf[hi])
    # deviation from previous frame
    dev2 = (abs(pfreq[pei] - hfreqp[hi]) if hfreqp[hi]>0 else fs)
    threshold = f0/3 + harmDevSlope * pfreq[pei]
    # accept peak if deviation is small
    if ((dev1<threshold) or (dev2<threshold)):
      # harmonic frequencies
      hfreq[hi] = pfreq[pei]
      # harmonic magnitudes
      hmag[hi] = pmag[pei]
      # harmonic phases
      hphase[hi] = pphase[pei]
    # increase harmonic index
    hi += 1
  return hfreq, hmag, hphase

def harmonicModelAnal(x, fs, w, N, H, t, nH, minf0, maxf0, f0et, harmDevSlope=0.01, minSineDur=.02):
  """
  Analysis of a sound using the sinusoidal harmonic model
  x: input sound;
  fs: sampling rate,
  w: analysis window;
  N: FFT size (minimum 512);
  t: threshold in negative dB,
  nH: maximum number of harmonics;
  minf0: minimum f0 frequency in Hz,
  maxf0: maximim f0 frequency in Hz;
  f0et: error threshold in the f0 detection (ex: 5),
  harmDevSlope: slope of harmonic deviation;
  minSineDur: minimum length of harmonics
  returns xhfreq, xhmag, xhphase: harmonic frequencies, magnitudes and phases
  """

  if (minSineDur <0):                                     # raise exception if minSineDur is smaller than 0
          raise ValueError("Minimum duration of sine tracks smaller than 0")

  hN = N//2                                               # size of positive spectrum
  hM1 = int(math.floor((w.size+1)/2))                     # half analysis window size by rounding
  hM2 = int(math.floor(w.size/2))                         # half analysis window size by floor
  x = np.append(np.zeros(hM2),x)                          # add zeros at beginning to center first window at sample 0
  x = np.append(x,np.zeros(hM2))                          # add zeros at the end to analyze last sample
  pin = hM1                                               # init sound pointer in middle of anal window
  pend = x.size - hM1                                     # last sample to start a frame
  fftbuffer = np.zeros(N)                                 # initialize buffer for FFT
  w = w / sum(w)                                          # normalize analysis window
  hfreqp = []                                             # initialize harmonic frequencies of previous frame
  f0t = 0                                                 # initialize f0 track
  f0stable = 0                                            # initialize f0 stable
  while pin<=pend:
    x1 = x[pin-hM1:pin+hM2]                               # select frame

    mX, pX = speech_model_dft.dftAnal(x1, w, N)
    # detect peak locations
    ploc = speech_model_sine.peakDetection(mX, t)
    # refine peak values
    iploc, ipmag, ipphase = speech_model_sine.peakInterp(mX, pX, ploc)
    # convert locations to Hez
    ipfreq = fs * iploc/N

    f0t = f0Twm(ipfreq, ipmag, f0et, minf0, maxf0, f0stable)  # find f0
    if ((f0stable==0)&(f0t>0)) \
                    or ((f0stable>0)&(np.abs(f0stable-f0t)<f0stable/5.0)):
      f0stable = f0t                                      # consider a stable f0 if it is close to the previous one
    else:
      f0stable = 0
    hfreq, hmag, hphase = harmonicDetection(ipfreq, ipmag, ipphase, f0t, nH, hfreqp, fs, harmDevSlope) # find harmonics
    hfreqp = hfreq
    if pin == hM1:                                        # first frame
      xhfreq = np.array([hfreq])
      xhmag = np.array([hmag])
      xhphase = np.array([hphase])
    else:                                                 # next frames
      xhfreq = np.vstack((xhfreq,np.array([hfreq])))
      xhmag = np.vstack((xhmag, np.array([hmag])))
      xhphase = np.vstack((xhphase, np.array([hphase])))
    pin += H                                              # advance sound pointer
  xhfreq = speech_model_sine.cleaningSineTracks(xhfreq, round(fs*minSineDur/H))     # delete tracks shorter than minSineDur
  return xhfreq, xhmag, xhphase
