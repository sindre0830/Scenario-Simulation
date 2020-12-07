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
	projectionMatrix = glm::perspective(glm::radians(60.f), static_cast<float>(width) / height, 0.1f, 1000.f);
    //set initial view matrix values
	camPos = glm::vec3(0.f, 2.f, 0.f);
	camFront = glm::vec3(0.f, 0.f, -1.f);
	camUp = glm::vec3(0.f, 1.f, 0.f);
	//set view matrix
	viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);
}

void Camera::updatePosition(GLFWwindow *window, double deltaTime) {
	const float camSpeed = 0.01f * deltaTime;
	//update camPos according to user input
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camPos += camSpeed * camFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camPos -= camSpeed * camFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camPos -= glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camPos += glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
	//update view matrix
	viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);
}

void Camera::updateDirection(double xpos, double ypos) {
	//set values according to the very first mouse movement
	if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
	//update values
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	//limit movment
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	//update direction
	yaw += xoffset;
	pitch += yoffset;
	//limit direction
	if(pitch > 89.f) pitch =  89.f;
	if(pitch < -89.f) pitch = -89.f;
	//update camFront according to new direction
	glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront = glm::normalize(direction);
	//update view matrix
	viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);
}

