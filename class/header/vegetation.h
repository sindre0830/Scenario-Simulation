#ifndef __VEGETATION_H
#define __VEGETATION_H
/* library */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
/**
 * @brief Vegetation class.
 * 
 */
class Vegetation {
    private:
        /* private data */
        GLuint shaderProgram = 0;
        GLuint VAO = 0;
        int meshAmount = 0, instanceIndex = 0;
    public:
        /* public functionality */
        ~Vegetation();
        Vegetation();
        void draw();
};
#endif
