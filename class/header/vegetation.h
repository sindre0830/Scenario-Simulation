#ifndef __VEGETATION_H
#define __VEGETATION_H
/* library */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
/**
 * @brief Terrain class.
 * 
 */
class Vegetation {
    private:
        /* private data */
        GLuint shaderProgram = 0;
        GLuint VAO = 0;
        int meshAmount = 0;
        /* private functionality */
        //std::vector<GLfloat> genCoordinates();
    public:
        /* public functionality */
        ~Vegetation();
        Vegetation();
        void draw();
};
#endif
