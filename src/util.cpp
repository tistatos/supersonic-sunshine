
#include "util.h"
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

#include <png.hpp>

namespace Util{

	Mesh createTriangleMesh() {
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		Vertex v0,v1,v2;
		v0.position = glm::vec3(0.0f,1.0f,0.0f);
		v1.position = glm::vec3(0.5f,0.0f,0.0f);
		v2.position = glm::vec3(-0.5f,0.0f,0.0f);
		v0.normal = glm::vec3(0.0f,0.0f,-1.0f);
		v1.normal = glm::vec3(0.0f,0.0f,-1.0f);
		v2.normal = glm::vec3(0.0f,0.0f,-1.0f);
		v0.textureCoordinates = glm::vec2(0.5f,1.0f);
		v1.textureCoordinates = glm::vec2(0.0f,0.0f);
		v2.textureCoordinates = glm::vec2(1.0f,0.0f);
		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);

		return Mesh(vertices, indices);
	}

	Mesh createQuad() {
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);
		Vertex v0,v1,v2,v3;
		v0.position = glm::vec3(-1.0f,-1.0f,0.0f);
		v1.position = glm::vec3(-1.0f,1.0f,0.0f);
		v2.position = glm::vec3(1.0f,1.0f,0.0f);
		v3.position = glm::vec3(1.0f,-1.0f,0.0f);
		v0.normal = glm::vec3(0.0f,0.0f,1.0f);
		v1.normal = glm::vec3(0.0f,0.0f,1.0f);
		v2.normal = glm::vec3(0.0f,0.0f,1.0f);
		v3.normal = glm::vec3(0.0f,0.0f,1.0f);
		v0.textureCoordinates = glm::vec2(0.0f,0.0f);
		v1.textureCoordinates = glm::vec2(0.0f,1.0f);
		v2.textureCoordinates = glm::vec2(1.0f,1.0f);
		v3.textureCoordinates = glm::vec2(1.0f,0.0f);

		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);

		return Mesh(vertices, indices);
	}

	Mesh createPlaneMesh(float x, float y) {
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(0);
		Vertex v0,v1,v2,v3;
		v0.position = glm::vec3(x,0.0f,y);
		v1.position = glm::vec3(x,0.0f,-y);
		v2.position = glm::vec3(-x,0.0f,-y);
		v3.position = glm::vec3(-x,0.0f,y);
		v0.normal = glm::vec3(0.0f,1.0f,0.0f);
		v1.normal = glm::vec3(0.0f,1.0f,0.0f);
		v2.normal = glm::vec3(0.0f,1.0f,0.0f);
		v3.normal = glm::vec3(0.0f,1.0f,0.0f);
		v0.textureCoordinates = glm::vec2(1.0f,0.0f);
		v1.textureCoordinates = glm::vec2(1.0f,1.0f);
		v2.textureCoordinates = glm::vec2(0.0f,0.0f);
		v2.textureCoordinates = glm::vec2(0.0f,1.0f);

		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);

		return Mesh(vertices, indices);
	}

	Mesh* createPlaneMeshPointer(float x, float y) {
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(0);
		Vertex v0,v1,v2,v3;
		v0.position = glm::vec3(x,0.0f,y);
		v1.position = glm::vec3(x,0.0f,-y);
		v2.position = glm::vec3(-x,0.0f,-y);
		v3.position = glm::vec3(-x,0.0f,y);
		v0.normal = glm::vec3(0.0f,1.0f,0.0f);
		v1.normal = glm::vec3(0.0f,1.0f,0.0f);
		v2.normal = glm::vec3(0.0f,1.0f,0.0f);
		v3.normal = glm::vec3(0.0f,1.0f,0.0f);
		v0.textureCoordinates = glm::vec2(1.0f,0.0f);
		v1.textureCoordinates = glm::vec2(1.0f,1.0f);
		v2.textureCoordinates = glm::vec2(0.0f,0.0f);
		v2.textureCoordinates = glm::vec2(0.0f,1.0f);

		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);

		return new Mesh(vertices, indices);
	}

	std::vector<Mesh*> loadFromFile(std::string path) {
		std::vector<Mesh*> meshes;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace );
		if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {// if is Not Zero {
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() <<std::endl;
			return std::vector<Mesh*>();
		}

		// Process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene, meshes);

		return meshes;
	}

	void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes) {
		std::cout << " process node  ";
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

	Mesh* processMesh(aiMesh* mesh, const aiScene* scene) {
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

				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.tangent = vector;

				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.bitangent = vector;
			}
			//texture coordinates
			// Wieeerd stuff with uv coords going on.
			// xy are swapped and x has to be mirrored
			//vertex.textureCoordinates = glm::vec2(
					//1.0 -mesh->mTextureCoords[0][i].y,
					//mesh->mTextureCoords[0][i].x);
			//FIXED: use assimp flag

			vertex.textureCoordinates = glm::vec2(
					mesh->mTextureCoords[0][i].y,
					mesh->mTextureCoords[0][i].x);
			vertices.push_back(vertex);
		}

		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for(GLuint i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for(GLuint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		aiMaterial* mtl = scene->mMaterials[mesh->mMaterialIndex];

		Textures meshTextures;
		aiString path;

		if(mtl->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			mtl->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			meshTextures.diffuse = createTexture(path.C_Str());
		}

		if(mtl->GetTextureCount(aiTextureType_HEIGHT) > 0) {
			mtl->GetTexture(aiTextureType_HEIGHT, 0, &path);
			meshTextures.normal = createTexture(path.C_Str());
		}

		if(mtl->GetTextureCount(aiTextureType_SPECULAR) > 0) {
			mtl->GetTexture(aiTextureType_SPECULAR, 0, &path);
			meshTextures.roughness = createTexture(path.C_Str());
		}

		Mesh* m = new Mesh(vertices, indices);
		m->textures = meshTextures;
		return m;
	}

	GLuint createTexture(std::string pngfile){

		png::image< png::rgb_pixel > image(pngfile);

		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		int width = image.get_width();
		int height = image.get_height();

		GLubyte* tex_data = new GLubyte[3*width*height];
		for(int j = 0; j < height; j++){
			for (int i = 0;i < width; i++){
				tex_data[(i+width*j)*3] = image[i][j].red;
				tex_data[(i+width*j)*3+1] = image[i][j].green;
				tex_data[(i+width*j)*3+2] = image[i][j].blue;
			}
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)tex_data);
		delete[] tex_data;
		return texID;
	}

	FBOstruct *initFBO(int width, int height, int int_method)
	{
		FBOstruct *fbo = new FBOstruct();

		fbo->width = width;
		fbo->height = height;

		// create objects
		glGenFramebuffers(1, &fbo->fb); // frame buffer id
		glBindFramebuffer(GL_FRAMEBUFFER, fbo->fb);
		glGenTextures(1, &fbo->texid);
		fprintf(stderr, "%i \n",fbo->texid);
		glBindTexture(GL_TEXTURE_2D, fbo->texid);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (int_method == 0)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo->texid, 0);

		// Renderbuffer
		// initialize depth renderbuffer
			glGenRenderbuffers(1, &fbo->rb);
			glBindRenderbuffer(GL_RENDERBUFFER, fbo->rb);
			glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, fbo->width, fbo->height );
			glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->rb );

		fprintf(stderr, "Framebuffer object %d\n", fbo->fb);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return fbo;
	}

	static int lastw = 0;
	static int lasth = 0;

	void useFBO(FBOstruct *out, FBOstruct *in1, FBOstruct *in2)
	{
		GLint curfbo;

	// This was supposed to catch changes in viewport size and update lastw/lasth.
	// It worked for me in the past, but now it causes problems to I have to
	// fall back to manual updating.
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &curfbo);
		if (curfbo == 0)
		{
			GLint viewport[4] = {0,0,0,0};
			GLint w, h;
			glGetIntegerv(GL_VIEWPORT, viewport);
			w = viewport[2] - viewport[0];
			h = viewport[3] - viewport[1];
			if ((w > 0) && (h > 0) && (w < 65536) && (h < 65536)) // I don't believe in 64k pixel wide frame buffers for quite some time
			{
				lastw = viewport[2] - viewport[0];
				lasth = viewport[3] - viewport[1];
			}
		}

		if (out != 0L)
			glViewport(0, 0, out->width, out->height);
		else
			glViewport(0, 0, lastw, lasth);

		if (out != 0L)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, out->fb);
			glViewport(0, 0, out->width, out->height);
		}
		else
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glActiveTexture(GL_TEXTURE7);
		if (in2 != 0L)
			glBindTexture(GL_TEXTURE_2D, in2->texid);
		else
			glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE6);
		if (in1 != 0L)
			glBindTexture(GL_TEXTURE_2D, in1->texid);
		else
			glBindTexture(GL_TEXTURE_2D, 0);
	}
}
