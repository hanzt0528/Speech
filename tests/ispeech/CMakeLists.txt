ADD_SUBDIRECTORY(utils)

set(test_functions
    ../../src/ispeech/utils/log.cc
  test_functions.cc
)

add_executable(test_functions ${test_functions}  ${HEADERS})
target_link_libraries(test_functions ${LIBS})

