import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *
from ispeech.transform import *


def test_Transform():

    t = Transform('t')
    f = t.fft()
    print(f)
    array = t.toArray();
    print(array)

    f2=t.fft2(array);
    print('f2')
    print(f2)
    
test_Transform()
