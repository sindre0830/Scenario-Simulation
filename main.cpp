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
#include "functionality.h"
#include "class/header/skybox.h"
#include "class/header/terrain.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
/* global data */
Camera* g_camera;
/**
 * Main program.
 */
int main() {
	//construct camera class and point adress to global pointer
	static Camera camera(1024, 1024);
	g_camera = &camera;
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
	GLFWwindow* window = glfwCreateWindow(1024, 1024, "Landscape", nullptr, nullptr);
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
	//load textures to be used in gameloop
	GLuint terrainTex = loadTexture("HeightMaps/Gjovik_Height MapLow.png", TERRAIN_TEXTURE);
	//construct terrain class
	Terrain terrain;
    //setup timer
	static double limitFPS = 1.f / 60.f;
	double lastTime = glfwGetTime(), nowTime = 0, timer = lastTime, deltaTime = 0.f;
	int counter = 0;
	//loop until user closes window
	while(!glfwWindowShouldClose(window)) {
		//delta time managment
		nowTime = glfwGetTime();
		deltaTime += (nowTime - lastTime) / limitFPS;
		lastTime = nowTime;
		//move camera
		g_camera->updatePosition(window, deltaTime);
		//processes all pending events
		glfwPollEvents();
		//for every frame reset background color buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		terrain.draw();

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
	glfwTerminate();
	return EXIT_SUCCESS;
}
