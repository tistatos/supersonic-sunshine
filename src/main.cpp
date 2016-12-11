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
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <png.hpp>

//stl
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>

//local
#include "SupersonicGui.h"
#include "shader.h"
#include "mesh.h"
#include "util.h"
#include "ltcMaps.h"
#include "Camera.h"
#include "arealight.h"

#include <glm/gtx/rotate_vector.hpp>

#define VSYNC true

Camera *camera;

double oldX;
double oldY;

void mouseMove(float xdelta, float ydelta) {
	float rotatespeed = 0.005f;
	camera->mFacing = glm::rotateY(camera->mFacing, -xdelta*rotatespeed);
	camera->mFacing.y -= rotatespeed*ydelta;
	glm::normalize(camera->mFacing);

}
void windowResizedCallback(GLFWwindow* window, int width, int height) {
	float aspect = (float)width/height;
	camera->setProjectionMatrix(glm::perspective((float)M_PI/3.0f, aspect, 0.001f, 1000.0f));
}

/* Inits a GLFW Window with OpenGL 3.3. Make sure glfwInit has been called */
GLFWwindow* createWindow() {
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetErrorCallback([](int code, const char* msg) {std::cout << "code: " << code << " msg: " << msg;});

	GLFWwindow* window = glfwCreateWindow(vidmode->width/2, vidmode->height/2, "SuperSonicSunshine", NULL, NULL);

	if (!window) {
		std::cout << "error creating window";
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(VSYNC);

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

	int width, height;
	glfwGetWindowSize(mWindow, &width, &height);
	camera = new Camera(width, height);
	camera->update();

	glfwGetCursorPos(mWindow,&oldX,&oldY);

	//fps counter bookkeeping
	float time_since_update, time, lastFrame = glfwGetTime();
	int frames = 0;


	Shader shader("../shaders/simple.vert","../shaders/simple.frag");

	GLuint normal = Util::createTexture("../assets/tile_normalmap.png");
	GLuint rough = Util::createTexture("../assets/tileroughness.png");
	//GLuint diffuse = Util::createTexture("../assets/tilealbedo.png");

	float roughness = 0.5f;



	SupersonicGUI* supergui = new SupersonicGUI(mWindow, [&supergui, &roughness](float val) {
			std::ostringstream out;
			out << std::setprecision(2) << val;
			supergui->textBox->setValue(out.str());
			roughness = val;
	} );

	Mesh plane = Util::createPlaneMesh(40.f, 40.f);
	plane.shader = &shader;
	//plane.textures.diffuse = diffuse;
	plane.textures.normal = normal;
	plane.textures.roughness = rough;
	LTC_t maps = loadLTCTextures();

	/** Nice screen while loading */
	glClearColor(0.f,0.f,0.3f,1.0);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(mWindow);


	std::vector<Mesh*> meshes = Util::loadFromFile("../assets/hallway.obj");
	for (Mesh* mesh : meshes) {
		mesh->shader = &shader;
	}

	//std::vector<Mesh*> meshes;
	//meshes.push_back(&plane);


	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 right(1.0f, 0.0f, 0.0f);


	AreaLight arealight(4.0f,4.0f, 4.0f);
	AreaLight arealight2(30.0f,1.5f, 7.0f);
	AreaLight arealight3(30.0f,1.5f, 7.0f);

	arealight2.setColor(glm::vec3(1.0,0.5,0.0));
	arealight3.setColor(glm::vec3(0.0,1.0,1.0));

	glm::mat4 lightMat(1.0f);
	lightMat = glm::rotate(lightMat, (float)(M_PI/2), right);
	lightMat = glm::translate(lightMat, glm::vec3(0.0f, -5.0f, -5.5f));

	glm::mat4 lightMat2(1.0f);	//
	lightMat2 = glm::translate(lightMat2, glm::vec3(-7.5f, 8.f, 15.f));
	lightMat2 = glm::rotate(lightMat2, (float)(M_PI/2), up);
	lightMat2 = glm::rotate(lightMat2, (float)(M_PI/2), right);
	arealight2.setMatrix(lightMat2);

	glm::mat4 lightMat3(1.0f);
	lightMat3 = glm::translate(lightMat3, glm::vec3(7.5f, 8.0f, 15.f));
	lightMat3 = glm::rotate(lightMat3, (float)(M_PI/2), up);
	lightMat3 = glm::rotate(lightMat3, (float)(-M_PI/2), right);
	arealight3.setMatrix(lightMat3);

	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float delta = 0.0f;
	bool usingMenu = false;
	bool pressed = false;
	while (!glfwWindowShouldClose(mWindow)) {
		time = glfwGetTime();
		delta = time - lastFrame;

		GLfloat cameraSpeed = 0.1f;
		if(glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT))
				cameraSpeed += 1.0f;
		if(glfwGetKey(mWindow, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(mWindow, GL_TRUE);
		if(glfwGetKey(mWindow,GLFW_KEY_W))
			camera->mPosition += cameraSpeed * camera->mFacing;
    if(glfwGetKey(mWindow,GLFW_KEY_S))
			camera->mPosition -= cameraSpeed * camera->mFacing;
    if(glfwGetKey(mWindow,GLFW_KEY_A))
			camera->mPosition -= glm::normalize(glm::cross(camera->mFacing, glm::vec3(0.f,1.0f,0.0f))) * cameraSpeed;
    if(glfwGetKey(mWindow,GLFW_KEY_D))
			camera->mPosition += glm::normalize(glm::cross(camera->mFacing, glm::vec3(0.f,1.0f,0.0f))) * cameraSpeed;
    if(glfwGetKey(mWindow,GLFW_KEY_F5))
			shader.reload();

		if(glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_RIGHT)) {
			if(!pressed) {
				pressed = true;
				if(usingMenu) {
					glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}
				else {
					glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}
				usingMenu = !usingMenu;
			}
		}
		else {
			pressed = false;
		}

		double x,y;
		glfwGetCursorPos(mWindow,&x,&y);
		if(!usingMenu) {
			mouseMove(x-oldX,y-oldY);
		}
		oldX = x;
		oldY = y;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		camera->update();

		if(supergui->shouldSpin) {
			glm::vec3 fwd(0.0f, 0.0f, 1.0f);
			lightMat = glm::rotate(lightMat, (float)(M_PI/4)*delta, fwd);
			arealight.setMatrix(lightMat);
		}

		bindTextures(&maps);

		for (Mesh* mesh : meshes) {
			mesh->setRoughness(roughness);
			arealight.use(*mesh->shader, 0);
			arealight2.use(*mesh->shader, 1);
			arealight3.use(*mesh->shader, 2);
			mesh->draw();
		}
		arealight.draw();
		arealight2.draw();
		arealight3.draw();


		supergui->draw();

		glfwSwapBuffers(mWindow);
		glfwPollEvents();

		/* Fps counter handling */
		frames++;
		if (time - time_since_update > 1.0f) {
			int fps = frames/(time - time_since_update);
			float ms = 1000.0f /frames;
			supergui->setFrameMetrics(fps, ms);
			time_since_update = time;
			supergui->refresh();
			frames = 0;
		}
		lastFrame = time;
	}

	glfwTerminate();
	return 0;
}
