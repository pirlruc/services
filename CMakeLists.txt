cmake_minimum_required(VERSION 3.14-3.18)

if(NOT DEFINED IMPROC_SUPERPROJECT_VERSION)
  set(IMPROC_SUPERPROJECT_VERSION 1.0.0)
endif()

project(
  improc_services
  VERSION   ${IMPROC_SUPERPROJECT_VERSION}
  LANGUAGES   CXX
)

# Add dependencies
if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
  # Project configuration
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"      CACHE PATH "Archive output dir.")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"      CACHE PATH "Library output dir.")
  set(CMAKE_PDB_OUTPUT_DIRECTORY     "${CMAKE_BINARY_DIR}/bin"      CACHE PATH "PDB (MSVC debug symbol)output dir.")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"      CACHE PATH "Executable/dll output dir.")
  set(CMAKE_DEBUG_POSTFIX d)
  option(BUILD_SHARED_LIBS           "Build using shared libraries" ON)
  option(IMPROC_WITH_TESTS 			     "Building library with tests"  ON)

  add_subdirectory(${PROJECT_SOURCE_DIR}/../infrastructure ${CMAKE_BINARY_DIR}/infrastructure)
endif()

message(STATUS "Build ${PROJECT_NAME} Version: ${PROJECT_VERSION}")
message(STATUS "Build Type : ${CMAKE_BUILD_TYPE}")
message(STATUS "Install Dir: ${CMAKE_INSTALL_PREFIX}")

add_library(
  ${PROJECT_NAME}
    
  ${PROJECT_SOURCE_DIR}/include
)
add_library(improc::services ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD           17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED  TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE        CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION                ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX          ${CMAKE_DEBUG_POSTFIX})

target_include_directories  (${PROJECT_NAME}   PRIVATE   ${PROJECT_SOURCE_DIR}/include)
target_include_directories  (${PROJECT_NAME}   INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                               $<INSTALL_INTERFACE:include> )

# Link dependencies
target_link_libraries       (${PROJECT_NAME}   PRIVATE   improc::infrastructure)
target_link_libraries       (${PROJECT_NAME}   INTERFACE improc::infrastructure)

# Tests configuration
if(IMPROC_WITH_TESTS)
  add_subdirectory(${PROJECT_SOURCE_DIR}/test     ${CMAKE_BINARY_DIR}/services_test)
endif()

if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
  add_subdirectory(${PROJECT_SOURCE_DIR}/version  ${CMAKE_BINARY_DIR}/version)  
endif()

# Install configuration
install(
  TARGETS     ${PROJECT_NAME}
  DESTINATION lib
  EXPORT      ${PROJECT_NAME}_target 
)
install(FILES   ${PROJECT_SOURCE_DIR}/include/improc/services/container/container.hpp             DESTINATION include/improc/services/container)
install(FILES   ${PROJECT_SOURCE_DIR}/include/improc/services/container/container.tpp             DESTINATION include/improc/services/container)
install(FILES   ${PROJECT_SOURCE_DIR}/include/improc/services/factory/factory_patern_policies.hpp DESTINATION include/improc/services/factory)
install(FILES   ${PROJECT_SOURCE_DIR}/include/improc/services/factory/factory_patern.hpp          DESTINATION include/improc/services/factory)
install(FILES   ${PROJECT_SOURCE_DIR}/include/improc/services/factory/factory_patern.tpp          DESTINATION include/improc/services/factory)
install(FILES   ${PROJECT_SOURCE_DIR}/include/improc/services/logger_services.hpp                 DESTINATION include/improc/services)

install(FILES   ${PROJECT_SOURCE_DIR}/include/improc/services/base_service.hpp              DESTINATION include/improc/services)
install(FILES   ${PROJECT_SOURCE_DIR}/include/improc/services/context.hpp                   DESTINATION include/improc/services)
install(FILES   ${PROJECT_SOURCE_DIR}/include/improc/services/factory.hpp                   DESTINATION include/improc/services)
install(FILES   ${PROJECT_SOURCE_DIR}/include/improc/services/sequence_service.hpp          DESTINATION include/improc/services)

add_executable(main_test main.cpp)
set_target_properties(main_test PROPERTIES CXX_STANDARD           17)
set_target_properties(main_test PROPERTIES CXX_STANDARD_REQUIRED  TRUE)
set_target_properties(main_test PROPERTIES LINKER_LANGUAGE        CXX)
target_link_libraries(main_test   PRIVATE   improc::services)
