#ifndef __TERRAIN_H
#define __TERRAIN_H
/* library */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
/**
 * @brief Terrain class.
 * 
 */
class Terrain {
    private:
        /* private data */
        GLuint shaderProgram = 0;
        GLuint VAO = 0;
        /* private functionality */
        std::vector<GLfloat> genCoordinates();
    public:
        /* public functionality */
        ~Terrain();
        Terrain();
        void draw();
};
#endif
