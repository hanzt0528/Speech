ispeech
========

ispeech 是一个音频基础库，功能包括对音频时域和频域的理解和处理，侧重于频域，在基础功能之上，实现了可用于语音的分析和合成的多个模型。这些基础功能，旨在为音乐和声音应用的实现提供一个起点和基础参考。

Installation
------------

## Building & Install & Example Python bindings

Building for c++:

cd Speech
mkdir build
cd build
cmake ..
make


Building for python:

cd bindings/python

python3 setup.py install
pip3 install -e .

or

./build.sh


	how to use ispeech:
	//code
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
	//end code.


	run python:

	python3 test_xxx.py


Quick start
-----------
目录结构介绍。
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


为对库先有个初步的认识，建议先从tests\python目录下的python样例程序开始熟悉。

对于具体c++类的功能，可以查看文档doc\html\index.html。

c++绑定python，查看文档doc\pybind11.pdf。

Docs:

文档生成方法，执行命令doxygen,如下：

doxygen Doxyfile.in

Asking for help
---------------

Versions
--------

How to contribute
-----------------
