cmake_minimum_required(VERSION 3.5.1)

# Download and build googletest
include(${CMAKE_MODULE_PATH}/BuildGoogleTest.cmake)

function(build_test SRCFILE)
  get_filename_component(src_name ${SRCFILE} NAME_WE)
  set(target "${src_name}")
  add_executable(${target} ${SRCFILE})
  add_dependencies(${target} gtest) # make sure gtest is built first

  target_link_libraries(
    ${target}
    PRIVATE
    ispeech
    ${GTEST_LIBRARIES}
    ${GMOCK_LIBRARIES}
    )

  target_include_directories(
    ${target}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/..
    ${GTEST_INCLUDE_DIR}
    ${GMOCK_INCLUDE_DIR}
    )
  target_compile_definitions(
    ${target}
    PRIVATE
    "DATA_TEST_DATADIR=\"${CMAKE_SOURCE_DIR}/src/data/test/testdata\""
    "DECODER_TEST_DATADIR=\"${CMAKE_SOURCE_DIR}/src/decoder/test\""
    "FEATURE_TEST_DATADIR=\"${CMAKE_SOURCE_DIR}/src/feature/test/data\""
    "MODULE_TEST_ARCHDIR=\"${CMAKE_SOURCE_DIR}/src/module/test\""
    )
  add_test(${target} ${target})
endfunction(build_test)

  # Criterion
  build_test(${CMAKE_SOURCE_DIR}/tests/ispeech/test_functions.cc)
  build_test(${CMAKE_SOURCE_DIR}/tests/ispeech/test_global.cc)
  build_test(${CMAKE_SOURCE_DIR}/tests/ispeech/utils/test_log.cc)
  build_test(${CMAKE_SOURCE_DIR}/tests/ispeech/source/test_SignalSource.cc)
  build_test(${CMAKE_SOURCE_DIR}/tests/ispeech/source/test_WaveFile.cc)
  build_test(${CMAKE_SOURCE_DIR}/tests/ispeech/source/test_Frame.cc)

  #window
  build_test(${CMAKE_SOURCE_DIR}/tests/ispeech/source/window/test_window.cc)

  #generator
  build_test(${CMAKE_SOURCE_DIR}/tests/ispeech/source/generator/test_generator.cc)

  #transform
  build_test(${CMAKE_SOURCE_DIR}/tests/ispeech/transform/test_spectrogram.cc)
  build_test(${CMAKE_SOURCE_DIR}/tests/ispeech/transform/test_pitchdetection.cc)
  #app
  build_test(${CMAKE_SOURCE_DIR}/tests/ispeech/app/test_song_pitchdetection.cc)
  build_test(${CMAKE_SOURCE_DIR}/tests/ispeech/app/test_score.cc)

  build_test(${CMAKE_SOURCE_DIR}/tests/algorithms/karaoke/test_midi.cc)
  build_test(${CMAKE_SOURCE_DIR}/tests/ispeech/model/test_ModelDFT.cc)
  build_test(${CMAKE_SOURCE_DIR}/tests/algorithms/karaoke/test_midi_parser.cc)
  build_test(${CMAKE_SOURCE_DIR}/tests/algorithms/karaoke/test_midi_make.cc)
  build_test(${CMAKE_SOURCE_DIR}/tests/algorithms/karaoke/test_lyrics.cc)
  build_test(${CMAKE_SOURCE_DIR}/tests/ispeech/function/test_dtw.cc)
  build_test(${CMAKE_SOURCE_DIR}/tests/ispeech/model/test_ModelHarmonic.cc)
  build_test(${CMAKE_SOURCE_DIR}/tests/ispeech/function/test_lpc.cc)

  # Data
