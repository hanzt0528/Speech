# 需要最小的CMake版本
cmake_minimum_required(VERSION 3.12.3)
# 工程的名称
project(Speech)


# ----------------------------- Setup -----------------------------
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------------------------- Configuration -----------------------------

option(SP_BUILD_LIBRARIES_ONLY "Build libraries only" OFF)
option(SP_LIBRARIES_USE_CUDA "Use CUDA in libraries-only build" ON)
option(SP_LIBRARIES_USE_KENLM "Use KenLM in libraries-only build" ON)
option(SP_LIBRARIES_USE_MKL "Use MKL in libraries-only build" ON)
option(SP_BUILD_FOR_PYTHON "Build Python bindings" ON)
option(SP_BUILD_TESTS "Build tests for this" ON)
option(SP_BUILD_EXAMPLES "Build examples for this" ON)
option(SP_BUILD_EXPERIMENTAL "Build internal experimental components for this" OFF)
option(SP_BUILD_SCRIPTS "Build internal scripts for this" OFF)
set(KENLM_MAX_ORDER 6 CACHE STRING "Maximum ngram order for KenLM")


# -fPIC则表明使用地址无关代码
add_compile_options(-fPIC)

SET(PROJECT_ROOT_PATH "${CMAKE_SOURCE_DIR}")

SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/")

SET(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib/")

INCLUDE_DIRECTORIES("/usr/local/include/opencv4/")

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/3rdparty/midifile/include/")

INCLUDE_DIRECTORIES("/usr/local/include/freetype2/")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/tests/")

Link_DIRECTORIES("/usr/local/lib/")

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(LIBS
    opencv_calib3d
    opencv_core
    opencv_dnn
    opencv_features2d
    opencv_flann
    opencv_gapi
    opencv_highgui
    opencv_imgcodecs
    opencv_imgproc
    opencv_ml
    opencv_objdetect
    opencv_photo
    opencv_stitching
    opencv_video
    opencv_videoio
    freetype
    )

#ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)


add_library(
  ispeech
  "./src/ispeech/def.cc"
  "./src/ispeech/utils/log.cc"
  "./src/ispeech/source/SignalSource.cpp"
  "./src/ispeech/source/WaveFileHandler.cpp"
  "./src/ispeech/source/WaveFile.cpp"
  "./src/ispeech/source/Frame.cpp"
  "./src/ispeech/source/FramesCollection.cpp"
  "./src/ispeech/source/PlainTextFile.cpp"
  "./src/ispeech/source/generator/Generator.cpp"
  "./src/ispeech/source/generator/PinkNoiseGenerator.cpp"
  "./src/ispeech/source/generator/SineGenerator.cpp"
  "./src/ispeech/source/generator/SquareGenerator.cpp"
  "./src/ispeech/source/generator/TriangleGenerator.cpp"
  "./src/ispeech/source/generator/WhiteNoiseGenerator.cpp"
  "./src/ispeech/source/window/BarlettWindow.cpp"
  "./src/ispeech/source/window/BlackmanWindow.cpp"
  "./src/ispeech/source/window/FlattopWindow.cpp"
  "./src/ispeech/source/window/GaussianWindow.cpp"
  "./src/ispeech/source/window/HammingWindow.cpp"
  "./src/ispeech/source/window/HannWindow.cpp"
  "./src/ispeech/source/window/TriangWindow.cpp"
  "./src/ispeech/source/window/BlackmanharrisWindow.cpp"
  "./src/ispeech/filter/MelFilter.cpp"
  "./src/ispeech/filter/MelFilterBank.cpp"
  "./src/ispeech/transform/Dft.cpp"
  "./src/ispeech/transform/AquilaFft.cpp"
  "./src/ispeech/transform/OouraFft.cpp"
  "./src/ispeech/transform/FftFactory.cpp"
  "./src/ispeech/transform/Dct.cpp"
  "./src/ispeech/transform/Mfcc.cpp"
  "./src/ispeech/transform/Spectrogram.cpp"
  "./src/ispeech/transform/PitchDetection.cpp"
  "./src/3rdparty/ooura/fft4g.c"
  "./src/algorithms/karaoke/midi.cpp"
  "./src/algorithms/karaoke/midi_parser.cc"
  "./src/algorithms/karaoke/midi_make.cc"
  "./src/algorithms/karaoke/karaoke.cc"
 
  "./src/3rdparty/midifile/src/Binasc.cpp"
  "./src/3rdparty/midifile/src/MidiEvent.cpp"
  "./src/3rdparty/midifile/src/MidiEventList.cpp"
  "./src/3rdparty/midifile/src/MidiFile.cpp"
  "./src/3rdparty/midifile/src/MidiMessage.cpp"
  "./src/3rdparty/midifile/src/Options.cpp"
  "./src/ispeech/model/ModelDFT.cc"
  "./src/ispeech/functions/gen_spec_sines.cc"
  "./src/ispeech/model/ModelSine.cc"
  "./src/ispeech/model/ModelSTFT.cc"
  "./src/ispeech/model/ModelHarmonic.cc"
  )



set_target_properties(
  ispeech
  PROPERTIES
  LINKER_LANGUAGE CXX
  CXX_STANDARD 11
  )

if (SP_BUILD_FOR_PYTHON)
  add_subdirectory(${PROJECT_SOURCE_DIR}/bindings/python)
endif ()

# 全局变量：CMAKE_SOURCE_DIR CMake的起始目录，即源码的根目录
# 全局变量：PROJECT_NAME 工程的名称
# 全局变量：PROJECT_SOURCE_DIR 工程的源码根目录的完整路径

