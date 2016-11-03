
#include <glm/glm.hpp>
#include <vector> 

#ifndef __MESH_H__
#define __MESH_H__

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

class Mesh{
public: 

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices){
        this->vertices = vertices;
        this->indices = indices;
        this->setupMesh();
    }   

    void draw() 
    {
        
        glBindVertexArray(this->vao);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }
    
private:
    GLuint vao, vbo, ibo;

    void setupMesh()
    {
        // Create buffers/arrays
        glGenVertexArrays(1, &this->vao);
        glGenBuffers(1, &this->vbo);
        glGenBuffers(1, &this->ibo);

        glBindVertexArray(this->vao);
       
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

        // Vertex Positions
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
       
        // Vertex Normals
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
       
        glBindVertexArray(0);
    }

};

#endif