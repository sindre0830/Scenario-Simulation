#ifndef __CAMERA_H
#define __CAMERA_H
/* library */
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
/**
 * @brief Camera class.
 * 
 */
class Camera {
    private:
        /* private data */
        glm::vec3 camFront, camPos, camUp;
        bool firstMouse = true, thirdPerson = false;
        float yaw, pitch, lastX, lastY;
        const float sensitivity = 0.05f;
    public:
        /* public data */
        glm::mat4 viewMatrix, projectionMatrix;
        /* public functionality */
        ~Camera();
        Camera(const int width, const int height);
        void updatePosition(GLFWwindow *window, double deltaTime);
        void updateDirection(double xpos, double ypos);
        void switchCamMode();
};
#endif
