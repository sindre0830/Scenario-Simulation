/**
 * Exam Autumn 2020 - Landscape rendering
 *
 * @author Sindre Eiklid (sindreik@stud.ntnu.no)
 *
 * @date 2020/12/10
 */
/* library */
#include "dictionary.h"
#include "camera.h"
#include "mapData.h"
#include "functionality.h"
#include "class/header/terrain.h"
#include "class/header/vegetation.h"
#include "class/header/entity.h"
#include "class/header/lightCycle.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
/* global data */
Camera* g_camera;
MapData* g_mapData;
/**
 * Main program.
 */
int main() {
	//construct mapData class and point adress to global pointer
	static MapData mapData;
	g_mapData = &mapData;
    //branch if GLFW isn't initialized and kill the application
	if(!glfwInit()) {
		std::cerr << "GLFW initialization failed.\n";
		std::cin.get();
		return EXIT_FAILURE;
	}
	//set window hints
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//create window
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Landscape", nullptr, nullptr);
	//set framebuffer size data
	int framebufferWidth = 0, framebufferHeight = 0;
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	//construct camera class and point adress to global pointer
	static Camera camera(framebufferWidth, framebufferHeight);
	g_camera = &camera;
	//declare framebuffer size callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//setting the OpenGL context to the window
	glfwMakeContextCurrent(window);
	//enable capture of cursor and focus it on the middle while hiding the icon
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//branch if window isn't created and kill the application
	if(window == nullptr) {
		std::cerr << "GLFW failed on window creation.\n";
		glfwTerminate();
		std::cin.get();
		return EXIT_FAILURE;
	}
	//branch if GLEW isn't initialized and kill the application
	if(glewInit() != GLEW_OK) {
		std::cerr << "GLEW initialization failuare.\n";
		glfwTerminate();
		std::cin.get();
		return EXIT_FAILURE;
	}
	//eanable capture of debug output
	enableDebug();
	//get initial cursor position
	glfwSetCursorPosCallback(window, mouse_callback);
	//enable MSAA
	glEnable(GL_MULTISAMPLE);
	//enable transparency on texture    //Might not need!!!
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//enable depth
	glEnable(GL_DEPTH_TEST);
	//load textures to be used in gameloop
	GLuint terrainTex = loadHeightMap("HeightMaps/Gjovik_Height MapLow.png", TERRAIN_TEXTURE);
	GLuint waterTex = loadTexture("NormalMapping/NormalMapping_Water.png", WATER_TEXTURE);
	//construct lightCycle class
	LightCycle lightCycle;
	//construct terrain class
	Terrain terrain;
	//construct vegetation class
	Vegetation vegetation;
	//construct entity class
	Entity entity;
    //setup timer
	static double limitFPS = 1.f / 30.f;
	double lastTime = glfwGetTime(), nowTime = 0, timer = lastTime, deltaTime = 0.f;
	int counter = 0;
	//set initial background color
	glClearColor(g_mapData->skyColor.r, g_mapData->skyColor.g, g_mapData->skyColor.b, 1.0f);
	//options
	bool pause = false;
	//loop until user closes window
	while(!glfwWindowShouldClose(window)) {
		//delta time managment
		nowTime = glfwGetTime();
		deltaTime += (nowTime - lastTime) / limitFPS;
		lastTime = nowTime;
		//move camera
		g_camera->updatePosition(window, deltaTime);
		//check for user input
		if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) pause = false;
		if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) pause = true;
		//processes all pending events
		glfwPollEvents();
		//for every frame reset background color buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //render VAOs
		terrain.draw();
		vegetation.draw();
		entity.draw();
		//update entity positions
		if(!pause && deltaTime >= 1.f) entity.mov();
		//branch every half a second
		if(glfwGetTime() - timer > 0.5f) {
			timer += 0.5f;
			//update the light cycle
			if(!pause) lightCycle.update();
			//set background color according to cycle
			glClearColor(g_mapData->skyColor.r, g_mapData->skyColor.g, g_mapData->skyColor.b, 1.0f);
			//randomize entity direction
			if(!pause) entity.randomizePathing();
		}
        //reset delta time 
		if(deltaTime >= 1.f) deltaTime -= 1.f;
		//go to next buffer
		glfwSwapBuffers(window);
		//break loop if 'ESC' key is pressed
		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
	}
	//clear memory before ending program
	glUseProgram(0);
	glDeleteTextures(1, &terrainTex);
	glDeleteTextures(1, &waterTex);
	glfwTerminate();
	return EXIT_SUCCESS;
}
