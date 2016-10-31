cmake_minimum_required(VERSION 2.8.12.2)

PROJECT(glm C CXX)
include(ExternalProject)

set(glm_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/external/glm")
set(glm_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${glm_INSTALL_DIR})

ExternalProject_Add(glm
  PREFIX ${glm_PREFIX}
  GIT_REPOSITORY http://github.com/g-truc/glm.git
  GIT_TAG 0.9.7.0
  CMAKE_ARGS ${glm_CMAKE_ARGS}
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_IN_SOURCE 1
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  )

set(GLM_INCLUDE_DIR ${glm_PREFIX}/src/glm PARENT_SCOPE)
