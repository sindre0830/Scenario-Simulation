#ifndef __CAMERA_H
#define __CAMERA_H
/* library */
#include <glm/glm.hpp>
/**
 * @brief Camera class.
 * 
 */
class Camera {
    private:
        /* private data */
        glm::vec3 camFront, camPos, camUp;
    public:
        /* public data */
        glm::mat4 viewMatrix, projectionMatrix;
        /* public functionality */
        ~Camera();
        Camera(const int width, const int height);

};
#endif
