import numpy as np 

def isPower2(num):
    """
    Check if num is power of two
    """
    return ((num & (num - 1)) == 0) and num > 0

# the length of x ,have be power of two.
def fft(x,N):

    if not(isPower2(N)):
        raise ValueError("FFT size(N) is not a pwer of 2")
   
    if N == 1:
        return x

    m = N/2-1

    even = np.array([])
    odd = np.array([])
    for i in range(N):
        if(i%2 == 1):
            odd =np.append(odd,x[i])
        else:
            even =np.append(even,x[i])

    Ek = fft(even,int(N/2))
    Ok = fft(odd,int(N/2))

    X = np.zeros(N,dtype = complex)
    for k in range(int(N/2)):
        X[k] = Ek[k] + np.exp(-1j*2*np.pi*k/N)*Ok[k]
        X[k+int(N/2)] = Ek[k]-np.exp(-1j*2*np.pi*k/N)*Ok[k]

    return X
#make x with zero-padding and up to N length,N must be power of two.
def fft_with_zeropadding(x,N):

    if not(isPower2(N)):
        raise ValueError("FFT size(N) is not a pwer of 2")

    N = int(N)
    M = len(x)
    hN = N//2 
    hM = (M+1)//2

    #zero-padding
    fftbuffer = np.zeros(N)
    fftbuffer[:hM] = x[hM-1:]
    fftbuffer[N-hM+1:] = x[:hM-1]

    #FFT
    X = fft(fftbuffer,N)

    X1 = np.zeros(N,dtype= complex)
    X1[:hN] = X[hN:]
    X1[N-hN:] = X[:hN]
    return X1

def ifft(x,N):

    if N == 1:
        return x

    m = N/2-1

    even = np.array([])
    odd = np.array([])
    for i in range(N):
        if(i%2 == 1):
            odd =np.append(odd,x[i])
        else:
            even =np.append(even,x[i])

    Ek = ifft(even,int(N/2))
    Ok = ifft(odd,int(N/2))

    X = np.zeros(N,dtype = complex)
    for k in range(int(N/2)):
        X[k] = Ek[k] + np.exp(1j*2*np.pi*k/N)*Ok[k]
        X[k+int(N/2)] = Ek[k]-np.exp(1j*2*np.pi*k/N)*Ok[k]

    return X
 
