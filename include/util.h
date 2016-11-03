
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Util{

	Mesh* createTriangleMesh() {
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
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh> meshes);

	std::vector<Mesh> loadFromFile(std::string path) {
				std::vector<Mesh> meshes;
				Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {// if is Not Zero {
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() <<std::endl;
			return std::vector<Mesh>();
		}

		// Process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene, meshes);

				return meshes;
	}
	void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh> meshes)
	{
		// Process each mesh located at the current node
		for(GLuint i = 0; i < node->mNumMeshes; i++) {
			// The node object only contains indices to index the actual objects in the scene.
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for(GLuint i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene, meshes);
		}
	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
		// Data to fill
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		// Walk through each of the mesh's vertices
		for(GLuint i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;
			// Normals
			if(mesh->HasNormals()) {
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.normal = vector;
			}

			vertices.push_back(vertex);
		}

		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for(GLuint i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for(GLuint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		return Mesh(vertices, indices);
	}
}
