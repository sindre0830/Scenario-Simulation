/* library */
#include "camera.h"
#include "dictionary.h"
#include <glm/gtc/matrix_transform.hpp>
/**
 * @brief Destroy the Camera:: Camera object.
 * 
 */
Camera::~Camera() {}
/**
 * @brief Construct a new Camera:: Camera object.
 * 
 * @param width 
 * @param height 
 */
Camera::Camera(const int width, const int height) {
	//set projection matrix
	projectionMatrix = glm::perspective(glm::radians(60.f), static_cast<float>(width) / height, 0.01f, 1000.f);
    //set initial view matrix values
	camPos = glm::vec3(0.f, 0.f, 0.f);
	camFront = glm::vec3(0.f, 0.f, -1.f);
	camUp = glm::vec3(0.f, 1.f, 0.f);
	//set view matrix
	viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);
}