/**
* @file main.cpp
* @author Erik Sandrén
* @date 31-10-2016
*/

//External libraries
#include <GL/glew.h>

#ifdef __APPLE__

#else
	#include <GL/gl.h>
#endif
#include <GLFW/glfw3.h>
//#include <nanogui/nanogui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure

//stl
#include <iostream>
#include <fstream>
#include <vector>

//local
#include "SupersonicGui.h"
#include "shader.h"
#include "mesh.h"
#include "util.h"

glm::mat4 cameraProjection;
glm::mat4 cameraView;

bool Import3DFromFile( const std::string& pFile) {

	Assimp::Importer importer;
	//check if file exists
	std::ifstream fin(pFile.c_str());
	if(!fin.fail()) {
		fin.close();
	}
	else {
		printf("Couldn't open file: %s\n", pFile.c_str());
		//printf("%s\n", importer.GetErrorString());
		return false;
	}

	const aiScene* scene;
	scene = importer.ReadFile( pFile, NULL);

	// If the import failed, report it
	if(!scene)
	{
		printf("%s\n", importer.GetErrorString());
		return false;
	}

	// Now we can access the file's contents.
	printf("Import of scene %s succeeded.",pFile.c_str());

	//aiVector3D scene_min, scene_max, scene_center;
	//get_bounding_box(&scene_min, &scene_max);
	//float tmp;
	//tmp = scene_max.x-scene_min.x;
	//tmp = scene_max.y - scene_min.y > tmp?scene_max.y - scene_min.y:tmp;
	//tmp = scene_max.z - scene_min.z > tmp?scene_max.z - scene_min.z:tmp;
	//scaleFactor = 1.f / tmp;

	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}

void windowResizedCallback(GLFWwindow* window, int width, int height) {
	float aspect = (float)width/height;
	cameraProjection = glm::perspective((float)M_PI/3.0f, aspect, 0.001f, 1000.0f);
}

/* Inits a GLFW Window with OpenGL 3.3. Make sure glfwInit has been called */
GLFWwindow* createWindow(){
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetErrorCallback([](int code, const char* msg){std::cout << "code: " << code << " msg: " << msg;});

	GLFWwindow* window = glfwCreateWindow(vidmode->width/2, vidmode->height/2, "snopp", NULL, NULL);

	if (!window) {
		std::cout << "error creating window";
		glfwTerminate();
		return nullptr;
	}

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	float aspect = (float)width/height;
	cameraProjection = glm::perspective((float)M_PI/3.0f, aspect, 0.001f, 1000.0f);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, windowResizedCallback);

	return window;
}


int main(int argc, char *argv[]) {
	if(!glfwInit())
		return -1;

	GLFWwindow* mWindow = createWindow();
	if (mWindow == nullptr)
		return -1;

	glewExperimental = GL_TRUE;
	glewInit();

	printf("GL Vendor:			%s\n", glGetString(GL_VENDOR));
	printf("GL Render:			%s\n", glGetString(GL_RENDERER));
	printf("GL Version:			%s\n", glGetString(GL_VERSION));

	SupersonicGUI* supergui = new SupersonicGUI(mWindow);

	cameraView = glm::lookAt(
			glm::vec3(0.f, 1.5f, -6.f), glm::vec3(0.f, 0.5f, 0.f), glm::vec3(0.f, 1.f, 0.f)
			);

	//fps counter bookkeeping
	float time_since_update, time = glfwGetTime();
	int frames = 0;


	Shader shader("../shaders/simple.vert","../shaders/simple.frag");

	Mesh* mesh = Util::createTriangleMesh();
	std::vector<Mesh> meshes = Util::loadFromFile("../assets/bunnySmall.obj");

	std::cout << "meshes loaded: " <<  meshes.size() << std::endl;

	while (!glfwWindowShouldClose(mWindow)) {

		if(glfwGetKey(mWindow, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(mWindow, GL_TRUE);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);
		glm::mat4 proj = cameraProjection * cameraView;
		glUniformMatrix4fv(glGetUniformLocation(shader, "mvp"), 1, GL_FALSE, glm::value_ptr(proj));
		for (Mesh mesh : meshes){
			mesh.draw();
		}
		glUseProgram(0);

		/* draw nano GUI */
		supergui->draw();
		/* Swap front and back buffers */
		glfwSwapBuffers(mWindow);

		/* Poll for and process events */
		glfwPollEvents();

		/* Fps counter handling */
		frames++;
		time = glfwGetTime();
		if (time - time_since_update > 1.0f){
			int fps = frames/(time - time_since_update);
			float ms = (time - time_since_update);
			supergui->setFrameMetrics(fps, ms);
			time_since_update = time;
			supergui->refresh();
			frames = 0;
		}

	}

	glfwTerminate();
	return 0;
}
