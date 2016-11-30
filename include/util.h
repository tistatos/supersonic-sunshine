

#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>


namespace Util{

	Mesh createTriangleMesh() ;

	Mesh createPlaneMesh(float x, float y) ;
	Mesh* createPlaneMeshPointer(float x, float y) ;

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes);

	std::vector<Mesh> loadFromFile(std::string path);

	GLuint createTexture(std::string pngfile);
}
