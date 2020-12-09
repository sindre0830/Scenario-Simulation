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
        std::vector<int> aerialLastDirection, groundLastDirection;
        std::vector<std::vector<int>> aerialGridPosition, groundGridPosition;
        bool aerialGridPossiblePath[200][200], groundGridPossiblePath[200][200];
        /* private functionality */
        int getRandomPath(const bool pathNorth, const bool pathWest, const bool pathSouth, const bool pathEast, const int index);
    public:
        /* public functionality */
        ~Entity();
        Entity();
        void draw();
        void mov();
};
#endif
