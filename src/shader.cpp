/**
* @file shader.cpp
* @author Erik Sandrén
* @date 03-11-2016
* @brief simple shader loading
*/


#include "shader.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

void getCompileErrors(GLuint shaderID, const char* filePath) {
	GLint status = 0;

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE)
		return;

	char log[4096];
	glGetShaderInfoLog(shaderID, sizeof(log), nullptr, log);
	std::cerr << "Shader Compile error - " << filePath << ":" << std::endl;
	std::cerr << log << std::endl;
}

void getLinkingError(GLuint programID, const char* vertexPath, const char* fragPath) {
	GLint status = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if(status == GL_TRUE) {
		std::cout << "Linking successful" << std::endl;
		return;
	}

	char log[4096];
	glGetProgramInfoLog(programID, sizeof(log), nullptr, log);
	std::cerr << "Program Link error - " << vertexPath << " and " << fragPath  << ":" << std::endl;
	std::cerr << log << std::endl;
}

std::string readFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint Shader::loadShader(const char *vertex_path, const char *fragment_path) {
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read shaders
    std::string vertShaderStr = readFile(vertex_path);
    std::string fragShaderStr = readFile(fragment_path);
    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;

    // Compile vertex shader
    std::cout << "Compiling vertex shader: " << vertex_path << std::endl;
    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);

    // Check vertex shader
		getCompileErrors(vertShader, vertex_path);

    // Compile fragment shader
    std::cout << "Compiling fragment shader: " << fragment_path << std::endl;
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);

    // Check fragment shader
		getCompileErrors(fragShader, fragment_path);

    std::cout << "Linking program" << std::endl;
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

		getLinkingError(program, vertex_path, fragment_path);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}
