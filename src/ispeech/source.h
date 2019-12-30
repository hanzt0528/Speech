/**
 * @file source.h
 *
 * This file is part of the ispeech library.
 * ispeech is free software, licensed under the MIT/X11 License. A copy of
 * the license is provided with the library in the LICENSE file.
 *
 * @package vfx_ispeech
 * @version 0.0.1-dev
 * @author Han Zhongtao
 * @date 2019-2019
 * @license
 * @since 0.0.1
 */


#ifndef ISPEECH_SOURCE_H
#define ISPEECH_SOURCE_H

#include "source/SignalSource.h"
#include "source/Frame.h"
#include "source/FramesCollection.h"
#include "source/PlainTextFile.h"
#include "source/RawPcmFile.h"
#include "source/WaveFile.h"
#include "source/WaveFileHandler.h"
#include "source/generator/Generator.h"
#include "source/generator/SineGenerator.h"
#include "source/generator/SquareGenerator.h"
#include "source/generator/TriangleGenerator.h"
#include "source/generator/PinkNoiseGenerator.h"
#include "source/generator/WhiteNoiseGenerator.h"
#include "source/window/BarlettWindow.h"
#include "source/window/BlackmanWindow.h"
#include "source/window/FlattopWindow.h"
#include "source/window/GaussianWindow.h"
#include "source/window/HammingWindow.h"
#include "source/window/HannWindow.h"
#include "source/window/RectangularWindow.h"
#include "source/window/TriangWindow.h"
#include "source/window/BlackmanharrisWindow.h"

#endif // ISPEECH_SOURCE_H
