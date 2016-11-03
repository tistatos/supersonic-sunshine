/**
* @file shader.h
* @author Erik Sandrén
* @date 03-11-2016
* @brief Simple shader loading
*/

#ifndef __SHADER_H__
#define __SHADER_H__

#include "GL/glew.h"

GLuint loadShader(const char *vertex_path, const char *fragment_path);

#endif
