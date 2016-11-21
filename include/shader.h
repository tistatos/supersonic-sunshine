/**
* @file shader.h
* @author Erik Sandrén
* @date 03-11-2016
* @brief Simple shader loading
*/

#ifndef __SHADER_H__
#define __SHADER_H__

#include "GL/glew.h"
#include <string>

class Shader{
public:
	GLuint loadShader(const char *vertex_path, const char *fragment_path);

	Shader() { };
	Shader(const char *vertex_path, const char *fragment_path){
		this->vertexPath = vertex_path;
		this->fragmentPath = fragment_path;
		this->program = this->loadShader(vertexPath.c_str(), fragmentPath.c_str());
	}

	void reload() {
		this->program = this->loadShader(vertexPath.c_str(), fragmentPath.c_str());
	}


	operator GLuint() { return program; }
private:
    GLuint program;
		std::string vertexPath;
		std::string fragmentPath;
};
#endif
