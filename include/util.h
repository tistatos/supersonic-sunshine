#ifndef __UTIL__
#define __UTIL__

#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>


namespace Util{

	typedef struct
	{
		GLuint texid;
		GLuint fb;
		GLuint rb;
		GLuint depth;
		int width, height;
	} FBOstruct;

	FBOstruct *initFBO(int width, int height, int int_method);
	void useFBO(FBOstruct *out, FBOstruct *in1, FBOstruct *in2);

	Mesh createTriangleMesh() ;
	Mesh createQuad();

	Mesh createPlaneMesh(float x, float y) ;
	Mesh* createPlaneMeshPointer(float x, float y) ;

	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes);

	std::vector<Mesh*> loadFromFile(std::string path);

	GLuint createTexture(std::string pngfile);
}
#endif
