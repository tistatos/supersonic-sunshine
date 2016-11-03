
#include "Mesh.h"

namespace Util{

    Mesh* createTriangleMesh(){
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        Vertex v0,v1,v2;
        v0.position = glm::vec3(0.0f,1.0f,0.0f);
        v1.position = glm::vec3(0.5f,0.0f,0.0f);
        v2.position = glm::vec3(-0.5f,0.0f,0.0f);
        v0.normal = glm::vec3(1.0f,0.0f,0.0f);
        v1.normal = glm::vec3(0.0f,1.0f,0.0f);
        v2.normal = glm::vec3(0.0f,0.0f,1.0f);
        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);

        Mesh* mesh = new Mesh(vertices, indices);

        return mesh;
    }

}