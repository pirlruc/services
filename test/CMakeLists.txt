cmake_minimum_required(VERSION 3.14-3.18)

include(FetchContent)
include(GoogleTest)

# Add external dependencies
# GOOGLETEST
if(DEFINED CMAKE_TOOLCHAIN_FILE)
  find_package(GTest CONFIG REQUIRED)
  set_target_properties(GTest::gtest      PROPERTIES FOLDER extern)
  set_target_properties(GTest::gtest_main PROPERTIES FOLDER extern)
else()
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY  https://github.com/google/googletest.git
    GIT_TAG         v1.17.0
    SOURCE_DIR      ${PROJECT_SOURCE_DIR}/test/external/googletest
  )
  set(BUILD_GMOCK OFF)
  FetchContent_MakeAvailable(googletest)
  add_library(GTest::gtest         ALIAS gtest)
  add_library(GTest::gtest_main    ALIAS gtest_main)
  set_target_properties(gtest      PROPERTIES FOLDER extern)
  set_target_properties(gtest_main PROPERTIES FOLDER extern)
endif()

mark_as_advanced(
  gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

configure_file(
  ${PROJECT_SOURCE_DIR}/test/${PROJECT_NAME_FILE}_test_config.hpp.in 
  ${PROJECT_SOURCE_DIR}/test/${PROJECT_NAME_FILE}_test_config.hpp
)

set(IMPROC_SERVICES_TEST_EXE ${PROJECT_NAME}_test)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(IMPROC_SERVICES_TEST_EXE ${PROJECT_NAME}_test${CMAKE_DEBUG_POSTFIX})
endif()

if(IMPROC_WITH_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(ENV{LLVM_PROFILE_FILE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

  find_program(LLVM_PROFDATA llvm-profdata-${IMPROC_COVERAGE_VERSION} REQUIRED)
  find_program(LLVM_COV      llvm-cov-${IMPROC_COVERAGE_VERSION}      REQUIRED)
  find_program(GENHTML       genhtml                                  REQUIRED)
  
  add_custom_target(
    ${PROJECT_NAME}_coverage

    # TODO: Add mcdc-coverage when clang version > 18.1. To do this we need to add -fcoverage-mcdc to the flags
    COMMAND ${LLVM_PROFDATA} merge --sparse ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/default.profraw 
                                   -o ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_coverage.profdata
    COMMAND ${LLVM_COV} export  --object ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/$<TARGET_FILE_NAME:${PROJECT_NAME}>
                                --ignore-filename-regex="external/*" 
                                --format=lcov --instr-profile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_coverage.profdata 
                                                             ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${IMPROC_SERVICES_TEST_EXE} 
                                                           > ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_coverage.info
    COMMAND ${GENHTML} --demangle-cpp -o ${PROJECT_NAME}_coverage ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_coverage.info
                       --branch-coverage
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
endif()

if(IMPROC_WITH_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

  find_program(GCOV    gcov-${IMPROC_COVERAGE_VERSION}  REQUIRED)
  find_program(LCOV    lcov                             REQUIRED)
  find_program(GENHTML genhtml                          REQUIRED)
  
  add_custom_target(
    ${PROJECT_NAME}_coverage

    # TODO: Add mcdc-coverage when gcc version > 14.2. To do this we need to add -fcondition-coverage to the flags
    COMMAND ${LCOV}    --directory ${CMAKE_BINARY_DIR}
                       --object ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/$<TARGET_FILE_NAME:${PROJECT_NAME}>
                       --capture --output-file ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_coverage.info
                       --branch-coverage
                       --gcov-tool ${GCOV}
    COMMAND ${LCOV}    --remove ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_coverage.info 'external/*'
                       --output-file ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_coverage.info                   
    COMMAND ${GENHTML} --demangle-cpp -o ${PROJECT_NAME}_coverage ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_coverage.info
                       --branch-coverage
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
endif()

set(
  IMPROC_SERVICES_TEST_FILES

  ${PROJECT_SOURCE_DIR}/test/test_logger_services.cpp
  ${PROJECT_SOURCE_DIR}/test/test_chain_responsibility_pattern.cpp
  ${PROJECT_SOURCE_DIR}/test/test_container.cpp
  ${PROJECT_SOURCE_DIR}/test/test_context.cpp
  ${PROJECT_SOURCE_DIR}/test/test_factory_pattern.cpp
  ${PROJECT_SOURCE_DIR}/test/test_variant_factory_pattern.cpp
  ${PROJECT_SOURCE_DIR}/test/test_factory.cpp
  ${PROJECT_SOURCE_DIR}/test/test_sequence_service.cpp
)

add_executable(${PROJECT_NAME}_test ${IMPROC_SERVICES_TEST_FILES})
set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_STANDARD           17)
set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_STANDARD_REQUIRED  TRUE)
set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_EXTENSIONS         OFF)
set_target_properties(${PROJECT_NAME}_test PROPERTIES LINKER_LANGUAGE        CXX)
set_target_properties(${PROJECT_NAME}_test PROPERTIES FOLDER                 ${PROJECT_SOURCE_DIR}/test)
set_target_properties(${PROJECT_NAME}_test PROPERTIES DEBUG_POSTFIX          ${CMAKE_DEBUG_POSTFIX})

target_compile_options(${PROJECT_NAME}_test PRIVATE ${TARGET_CXX_FLAGS})
target_link_options   (${PROJECT_NAME}_test PRIVATE ${TARGET_CXX_FLAGS})

target_include_directories(${PROJECT_NAME}_test  PRIVATE ${PROJECT_SOURCE_DIR}/test)

target_link_libraries(${PROJECT_NAME}_test  PRIVATE ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_test  PRIVATE GTest::gtest GTest::gtest_main)

gtest_discover_tests(
  ${PROJECT_NAME}_test
  DISCOVERY_MODE PRE_TEST
  DISCOVERY_TIMEOUT 30
  WORKING_DIRECTORY                           ${PROJECT_SOURCE_DIR}/test
  PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY    "${PROJECT_SOURCE_DIR}/test"
)
