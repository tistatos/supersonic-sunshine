cmake_minimum_required(VERSION 2.8.12.2)

PROJECT(GLEW C CXX)
include(ExternalProject)

set(GLEW_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/external/GLEW")

ExternalProject_Add(GLEW
  PREFIX ${GLEW_PREFIX}
  GIT_REPOSITORY http://github.com/nigels-com/glew.git
  GIT_TAG glew-2.0.0
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_IN_SOURCE 1
  BUILD_COMMAND make extensions
  INSTALL_COMMAND make all
  )

set(GLEW_INCLUDE_DIR ${GLEW_PREFIX}/src/GLEW PARENT_SCOPE)
set(GLEW_LIBRARIES ${GLEW_PREFIX}/src/GLEW/lib/libGLEW.so PARENT_SCOPE)
