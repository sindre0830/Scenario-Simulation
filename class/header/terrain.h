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
        GLuint terrainShaderProgram = 0, waterShaderProgram = 0;
        GLuint terrainVAO = 0, waterVAO = 0;
        int meshAmount = 0;
        /* private functionality */
        std::vector<GLfloat> genCoordinates();
    public:
        /* public functionality */
        ~Terrain();
        Terrain();
        void draw();
};
#endif
