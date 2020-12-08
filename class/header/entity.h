#ifndef __ENTITY_H
#define __ENTITY_H
/* library */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
/**
 * @brief Terrain class.
 * 
 */
class Entity {
    private:
        /* private data */
        GLuint aerielShaderProgram = 0, groundShaderProgram = 0;
        GLuint aerielVAO = 0, groundVAO = 0;
        int aerielMeshAmount = 0, groundMeshAmount = 0;
        glm::vec3 aerialInstancePos[42], groundInstancePos[18];
        int aerialInstanceIndex = 0, groundInstanceIndex = 0;
        std::vector<int> aerialLastPath, groundLastPath;
        /* private functionality */
        //std::vector<GLfloat> genCoordinates();
    public:
        /* public functionality */
        ~Entity();
        Entity();
        void draw();
        void mov();
};
#endif
