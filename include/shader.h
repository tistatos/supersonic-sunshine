/**
* @file shader.h
* @author Erik Sandrén
* @date 03-11-2016
* @brief Simple shader loading
*/

#ifndef __SHADER_H__
#define __SHADER_H__

#include "GL/glew.h"
class Shader{
public:

GLuint loadShader(const char *vertex_path, const char *fragment_path);
    
    Shader(const char *vertex_path, const char *fragment_path){
        this->program = this->loadShader(vertex_path, fragment_path);
    }

    operator GLuint() { return program; }

    private: 
    GLuint program; 

};
#endif
