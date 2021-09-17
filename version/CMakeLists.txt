cmake_minimum_required(VERSION 3.14-3.18)

# Create executable file to get version of library
configure_file(
  ${PROJECT_SOURCE_DIR}/version/version_config.hpp.in 
  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_config.hpp
)

add_executable(
  ${PROJECT_NAME}_version

  ${PROJECT_SOURCE_DIR}/version/main.cpp
)
set_target_properties     (${PROJECT_NAME}_version PROPERTIES CXX_STANDARD           11)
set_target_properties     (${PROJECT_NAME}_version PROPERTIES CXX_STANDARD_REQUIRED  TRUE)
set_target_properties     (${PROJECT_NAME}_version PROPERTIES DEBUG_POSTFIX          ${CMAKE_DEBUG_POSTFIX})
target_include_directories(${PROJECT_NAME}_version PRIVATE    ${CMAKE_BINARY_DIR})

# Install configuration
install(TARGETS ${PROJECT_NAME}_version                         DESTINATION bin)
install(FILES   ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_config.hpp  DESTINATION include)
