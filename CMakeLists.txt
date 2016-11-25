cmake_minimum_required(VERSION 2.8.12.2)
SET(PROJECT_NAME supersonic-sunshine)

#set project
project(${PROJECT_NAME})

#variables for directories
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_CXX_DIR ${PROJECT_SOURCE_DIR}/src)
set(PROJECT_EXT_DIR ${PROJECT_SOURCE_DIR}/external)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
		message( FATAL_ERROR "Please make a build directory" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

#compiler flags
SET(CMAKE_CXX_FLAGS "-std=c++11")

if(CMAKE_COMPILER_IS_GNUCXX)
add_definitions(
	-g
	-Wall
	-DGLEW_STATIC
 -D_CRT_SECURE_NO_WARNINGS
)
endif()

#git is used to download glm
find_package(OpenGL REQUIRED)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#fix all opengl libs
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	-lGL -lXrandr -lXext -lX11 -lrt -lXxf86vm
	${CMAKE_DL_LIBS}
)
#add unix macros
add_definitions(-D_UNIX)
endif()

#Get external dependencies
add_subdirectory(
	${PROJECT_EXT_DIR}
)

set(ext_includes
	${GLM_INCLUDE_DIR}
	${GLFW_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${NANOGUI_INCLUDE_DIR}
	${ASSIMP_INCLUDE_DIR}
	${PNGPP_INCLUDE_DIR}
)

include_directories(
	${ext_includes}
	${PROJECT_INCLUDE_DIR}
)

set(EXT_DEPS
	glm
	GLEW
	nanoGUI
	assimp
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	${GLFW_LIBRARIES}
	${GLEW_LIBRARIES}
	${NANOGUI_LIBRARIES}
	${ASSIMP_LIBRARIES}
	png
)

add_subdirectory(${PROJECT_CXX_DIR})
add_executable(${PROJECT_NAME} ${MAIN_SOURCE})
add_dependencies(${PROJECT_NAME} ${EXT_DEPS})
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})
