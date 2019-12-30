

set(iSpeech_Test_SOURCES
  ../../../src/ispeech/utils/log.cc
  test_log.cc
)

add_executable(test_log ${iSpeech_Test_SOURCES}  ${HEADERS})
target_link_libraries(test_log ${LIBS})
