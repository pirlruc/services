cmake_minimum_required(VERSION 3.14-3.18)

include(FetchContent)
include(GoogleTest)

# Add external dependencies
# GOOGLETEST
FetchContent_Declare(
  googletest
  GIT_REPOSITORY  https://github.com/google/googletest.git
  GIT_TAG         release-1.11.0
  SOURCE_DIR      ${PROJECT_SOURCE_DIR}/test/external/googletest
)
set(BUILD_GMOCK OFF)
FetchContent_MakeAvailable(googletest)

mark_as_advanced(
  gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

set_target_properties(gtest      PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)

# Create file to get folder with test
configure_file(
  ${PROJECT_SOURCE_DIR}/test/${PROJECT_NAME}_test_config.hpp.in 
  ${PROJECT_SOURCE_DIR}/test/${PROJECT_NAME}_test_config.hpp
)

if(IMPROC_WITH_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
  set(ENV{LLVM_PROFILE_FILE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

  find_program(LLVM_PROFDATA llvm-profdata-14 REQUIRED)
  find_program(LLVM_COV      llvm-cov-14      REQUIRED)
  find_program(GENHTML       genhtml          REQUIRED)
  
  add_custom_target(
    ${PROJECT_NAME}_coverage

    COMMAND ${LLVM_PROFDATA} merge --sparse ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/default.profraw 
                                   -o ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_coverage.profdata
    COMMAND ${LLVM_COV} export --format=lcov --instr-profile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_coverage.profdata 
                                                             ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}_testd 
                                                           > ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_coverage.lcov
    COMMAND ${GENHTML} --demangle-cpp -o ${PROJECT_NAME}_coverage ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_coverage.lcov
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
endif()

if(IMPROC_WITH_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Og --coverage")
  set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

  find_program(GCOV    gcov-11 REQUIRED)
  find_program(LCOV    lcov    REQUIRED)
  find_program(GENHTML genhtml REQUIRED)
  
  add_custom_target(
    ${PROJECT_NAME}_coverage

    COMMAND ${LCOV}    --directory ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_test/CMakeFiles/${PROJECT_NAME}_test.dir
                       --capture --output-file ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_coverage.info
                       --gcov-tool ${GCOV}
    COMMAND ${GENHTML} --demangle-cpp -o ${PROJECT_NAME}_coverage ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_coverage.info
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
endif()

add_executable(
  ${PROJECT_NAME}_test
    
  ${PROJECT_SOURCE_DIR}/test/test_logger_services.cpp
  ${PROJECT_SOURCE_DIR}/test/test_chain_responsibility_pattern.cpp
  ${PROJECT_SOURCE_DIR}/test/test_container.cpp
  ${PROJECT_SOURCE_DIR}/test/test_factory_pattern.cpp
  ${PROJECT_SOURCE_DIR}/test/test_variant_factory_pattern.cpp
  ${PROJECT_SOURCE_DIR}/test/test_duplicated_service.cpp
  ${PROJECT_SOURCE_DIR}/test/test_several_outputs.cpp
  ${PROJECT_SOURCE_DIR}/test/test_services_main.cpp
)
set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_STANDARD           17)
set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_STANDARD_REQUIRED  TRUE)
set_target_properties(${PROJECT_NAME}_test PROPERTIES LINKER_LANGUAGE        CXX)
set_target_properties(${PROJECT_NAME}_test PROPERTIES FOLDER                 ${PROJECT_SOURCE_DIR}/test)
set_target_properties(${PROJECT_NAME}_test PROPERTIES DEBUG_POSTFIX          ${CMAKE_DEBUG_POSTFIX})

target_include_directories(${PROJECT_NAME}_test  PRIVATE ${PROJECT_SOURCE_DIR}/test)

target_link_libraries(${PROJECT_NAME}_test  PRIVATE ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_test  PRIVATE gtest gtest_main)

gtest_discover_tests(
  ${PROJECT_NAME}_test
  WORKING_DIRECTORY                           ${PROJECT_SOURCE_DIR}/test
  PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY    "${PROJECT_SOURCE_DIR}/test"
)
