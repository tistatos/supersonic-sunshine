cmake_minimum_required(VERSION 2.8.12.2)

PROJECT(pngpp C CXX)
include(ExternalProject)

set(PNGPP_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/external/pngpp")
set(PNGPP_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PNGPP_INSTALL_DIR})

ExternalProject_Add(pngpp
	PREFIX ${PNGPP_PREFIX}
	SVN_REPOSITORY http://svn.sv.nongnu.org/svn/pngpp/trunk/
	UPDATE_COMMAND ""
	CONFIGURE_COMMAND ""
	BUILD_IN_SOURCE 1
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)

set(PNGPP_INCLUDE_DIR ${PNGPP_PREFIX}/src/pngpp PARENT_SCOPE)
