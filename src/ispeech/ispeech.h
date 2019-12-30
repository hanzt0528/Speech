#ifndef _ISPEECH_H
#define _ISPEECH_H

/** \mainpage

  \section introduction Introduction

 ispeech 是一个音频基础库，功能包括对音频时域和频域的理解和处理，侧重于频域，在基础功能之上，实现了可用于语音的分析和合成的多个模型。这些基础功能，旨在为音乐和声音应用的实现提供一个起点和基础参考。

  \section basics Basics

目录结构介绍:

bin：编译的c++测试程序。

lib：编译的音频库文件。

cmake：一些cmake脚本。

bindings：其他语言的绑定类，比如python。

tests：单元测试程序目录，包括python和c++测试程序。

src：实现的源代码目录，包括c++和python语言。

src\3rdparty:引用的第三方库目录。

src\algorithms:在基础库基础上实现的具体功能算法目录。

src\ispeech:语音基础处理c++库

src\ispeech\source:语音信号源目录，比如wav读写，窗函数生成等。

src\ispeech\transform:各种变换目录，比如DCT，DFT，FFT，MFCC等。

src\ispeech\functions:基础函数目录。

src\ispeech\utils:工具目录，比如日志等。

src\ispeech\model:模型目录，比如DFT，STFT，SINE，HARMONIC。

src\ispeech\filter:滤镜目录，比如BankFilter。

src\python:语音基础处理python库,比如source,fft，dft，harmonic,sine,stft等。


  \section Building

  building for c++:

  cd Speech

  mkdir build

  cd build

  cmake ..

  make


building for python:

  cd bindings/python

  python3 setup.py install

pip3 install -e .

  or

  cd bindings/python

  ./build.sh


  Here is a  most common python code for ispeech:

  \code
	from ispeech.source import *
	from ispeech.transform import *

	N = 256;
	sine = SineGenerator(44100)
	sine.setFrequency(5000.0)
	sine.setAmplitude(1.0)
	sine.setPhase(1.0)
	sine.generate(N)

	dft = Dft(N)

	spectrum_sine=fft.fft(sine)

  \endcode

  \subsection example Example

  Several examples of C++ programs are available in the  \p tests/ispeech
  directories of the source tree.

  Some examples:
  - @ref tests/ispeech/test_global.cc
  - @ref tests/ispeech/test_functions.cc

\section Docs

doxygen Doxyfile.in


  \section download Download

  Latest versions, further documentation, examples, wiki, and mailing lists can
  be found at GitLab .

 */


/** @file ispeech.h Global ispeech include file.

  You will want to include this file as:

  @code
    #include <ispeech/ispeech.h>
  @endcode

 */

#include "./utils/log.h"
#include "./global.h"
#include "./functions.h"
#include "./source.h"
#include "./transform.h"
#include "./filter.h"
#include "./model.h"

#endif //end _ISPEECH_H
