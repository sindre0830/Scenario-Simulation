/* library */
#include "header/entity.h"
#include "shader/modelShader.h"
#include "dictionary.h"
#include "camera.h"
#include "mapData.h"
#include "functionality.h"
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
/* global data */
extern Camera *g_camera;
extern MapData* g_mapData;

Entity::~Entity() {
    glDeleteProgram(aerielShaderProgram);
    glDeleteProgram(groundShaderProgram);
    destroyVAO(aerielVAO);
    destroyVAO(groundVAO);
}

Entity::Entity() {
    //fill in possible paths with true, this will be updated when getting each instance
    for(int i = 0; i < g_mapData->gridHeight; i++) {
        for(int j = 0; j < g_mapData->gridWidth; j++) {
            groundGridPossiblePath[i][j] = true;
            aerialGridPossiblePath[i][j] = true;
        }
    }
    //construct aeriel entity
    aerielShaderProgram = compileShader(modelVertexShader, modelFragmentShader);
	glUseProgram(aerielShaderProgram);
    aerielVAO = loadModel("Objects/Eagle/", "Eagle02.obj", aerielMeshAmount);
	//change color to light brown;
	glUniform3fv(glGetUniformLocation(aerielShaderProgram, "u_objectColor"), 1, glm::value_ptr(glm::vec3(0.28f, 0.16f, 0.04f)));
    //set projection matrix uniform
    glUniformMatrix4fv(glGetUniformLocation(aerielShaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->projectionMatrix));
    //rotate and scale entity
    glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.002f));
    glUniformMatrix4fv(glGetUniformLocation(aerielShaderProgram, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    //get all possible aerial enitites
    for(int i = 0; i < g_mapData->gridHeight; i += 20) {
        for(int j = 0; j < g_mapData->gridWidth; j += 20) {
            //branch if depth is higher than 75 (in gray and white area)
            if(g_mapData->gridElement[std::make_pair(i, j)][0][Z] > 75 / 255.f) {
                glm::vec3 translation;
                //multiply the value by 500 to compensate for the bird scaling(0.002f * 500.f = 1), then multiply by 2 because of the terrain scalling(2.f)
                translation.x = g_mapData->gridElement[std::make_pair(i, j)][0][X] * (500.f * 2.f);
                translation.y = g_mapData->gridElement[std::make_pair(i, j)][0][Y] * (500.f * 2.f);
                translation.z = g_mapData->gridElement[std::make_pair(i, j)][0][Z] * (500.f * 2.f) + 200.f;
                aerialInstancePos[aerialInstanceIndex] = translation;
                aerialInstanceIndex++;
                //remember initial position to compute coalition when moving
                aerialGridPosition.push_back({i, j});
                aerialGridPossiblePath[i][j] = false;
            }
        }
    }
    //fill in random paths
    std::vector<int> filler(aerialInstanceIndex, 0);
    for(int i = 0; i < aerialInstanceIndex; i++) {
        filler[i] = randomIndex(0, 3);
    }
    aerialLastDirection = filler;
    //send initial position to uniform
    for(unsigned int i = 0; i < aerialInstanceIndex; i++) {
        std::string strBuffer = "offsets[" + std::to_string(i) + "]";
        const char *c_str = strBuffer.c_str();
        glUniform3fv(glGetUniformLocation(aerielShaderProgram, c_str), 1, glm::value_ptr(aerialInstancePos[i]));
    }
    //construct ground entity
    groundShaderProgram = compileShader(modelVertexShader, modelFragmentShader);
	glUseProgram(groundShaderProgram);
    groundVAO = loadModel("Objects/Deer2/", "deer.obj", groundMeshAmount);
	//change color to brown;
	glUniform3fv(glGetUniformLocation(groundShaderProgram, "u_objectColor"), 1, glm::value_ptr(glm::vec3(0.37f, 0.34f, 0.18f)));
    //set projection matrix uniform
    glUniformMatrix4fv(glGetUniformLocation(groundShaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->projectionMatrix));
    //rotate and scale entity
    modelMatrix = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02f));
    glUniformMatrix4fv(glGetUniformLocation(groundShaderProgram, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    //get all possible ground enitites
    for(int i = 0; i < g_mapData->gridHeight; i += 20) {
        for(int j = 0; j < g_mapData->gridWidth; j += 20) {
            //branch if depth is between 40 and 60 (in green area)
            if(g_mapData->gridElement[std::make_pair(i, j)][0][Z] > 40 / 255.f && g_mapData->gridElement[std::make_pair(i, j)][0][2] < 60 / 255.f) {
                glm::vec3 translation;
                //multiply the value by 50 to compensate for the deer scaling(0.02f * 50.f = 1), then multiply by 2 because of the terrain scaling(2.f)
                translation.x = g_mapData->gridElement[std::make_pair(i, j)][0][X] * (50.f * 2.f);
                translation.y = g_mapData->gridElement[std::make_pair(i, j)][0][Y] * (50.f * 2.f);
                translation.z = g_mapData->gridElement[std::make_pair(i, j)][0][Z] * (50.f * 2.f);
                groundInstancePos[groundInstanceIndex] = translation;
                groundInstanceIndex++;
                //remember initial position to compute coalition when moving
                groundGridPosition.push_back({i, j});
                groundGridPossiblePath[i][j] = false;
            }
        }
    }
    //fill in random paths
    filler = std::vector<int>(groundInstanceIndex, 0);
    for(int i = 0; i < groundInstanceIndex; i++) {
        filler[i] = randomIndex(0, 3);
    }
    groundLastDirection = filler;
    //send initial position to uniform
    for(unsigned int i = 0; i < groundInstanceIndex; i++) {
        std::string strBuffer = "offsets[" + std::to_string(i) + "]";
        const char *c_str = strBuffer.c_str();
        glUniform3fv(glGetUniformLocation(groundShaderProgram, c_str), 1, glm::value_ptr(groundInstancePos[i]));
    }
    glUseProgram(0);
}

void Entity::draw() {
    //dynamic light direction
    glm::vec3 lightDirection(sin(glfwGetTime()), 3.f, cos(glfwGetTime()));
    //render aerial entites
    glUseProgram(aerielShaderProgram);
    glBindVertexArray(aerielVAO);
    glUniform3fv(glGetUniformLocation(aerielShaderProgram, "u_lightDirection"), 1, glm::value_ptr(lightDirection));
    glUniformMatrix4fv(glGetUniformLocation(aerielShaderProgram, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));
    glDrawArraysInstanced(GL_TRIANGLES, 6, aerielMeshAmount, aerialInstanceIndex);
    //render ground entites
    glUseProgram(groundShaderProgram);
    glBindVertexArray(groundVAO);
    glUniform3fv(glGetUniformLocation(groundShaderProgram, "u_lightDirection"), 1, glm::value_ptr(lightDirection));
    glUniformMatrix4fv(glGetUniformLocation(groundShaderProgram, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));
    glDrawArraysInstanced(GL_TRIANGLES, 6, groundMeshAmount, groundInstanceIndex);
    glUseProgram(0);
}

void Entity::mov() {
    //move aerial instances
    glUseProgram(aerielShaderProgram);
    for(unsigned int i = 0; i < aerialInstanceIndex; i++) {
        bool
            pathNorth = aerialGridPosition[i][COLUMN] + 1 < g_mapData->gridHeight && aerialGridPossiblePath[aerialGridPosition[i][COLUMN] + 1][aerialGridPosition[i][ROW]],
            pathWest = aerialGridPosition[i][ROW] - 1 >= 0 && aerialGridPossiblePath[aerialGridPosition[i][COLUMN]][aerialGridPosition[i][ROW] - 1],
            pathSouth = aerialGridPosition[i][COLUMN] - 1 >= 0 && aerialGridPossiblePath[aerialGridPosition[i][COLUMN] - 1][aerialGridPosition[i][ROW]],
            pathEast = aerialGridPosition[i][ROW] + 1 < g_mapData->gridWidth && aerialGridPossiblePath[aerialGridPosition[i][COLUMN]][aerialGridPosition[i][ROW] + 1];
        //branch if the entity can't move in the same path as before
        if(!((aerialLastDirection[i] == NORTH && pathNorth) || (aerialLastDirection[i] == WEST && pathWest) || (aerialLastDirection[i] == SOUTH && pathSouth) || (aerialLastDirection[i] == EAST && pathEast))) {
            //get a random possible path
            aerialLastDirection[i] = getRandomPath(pathNorth, pathWest, pathSouth, pathEast, i);
        }
        //update position according to current direction
        aerialGridPossiblePath[aerialGridPosition[i][COLUMN]][aerialGridPosition[i][ROW]] = true;
        switch(aerialLastDirection[i]) {
            case NORTH:
                aerialInstancePos[i].y = (g_mapData->gridElement[std::make_pair(++aerialGridPosition[i][COLUMN], aerialGridPosition[i][ROW])][0][Y] * (500.f * 2.f));
                break;
            case WEST:
                aerialInstancePos[i].x = (g_mapData->gridElement[std::make_pair(aerialGridPosition[i][COLUMN], --aerialGridPosition[i][ROW])][0][X] * (500.f * 2.f));
                break;
            case SOUTH:
                aerialInstancePos[i].y = (g_mapData->gridElement[std::make_pair(--aerialGridPosition[i][COLUMN], aerialGridPosition[i][ROW])][0][Y] * (500.f * 2.f));
                break;
            case EAST:
                aerialInstancePos[i].x = (g_mapData->gridElement[std::make_pair(aerialGridPosition[i][COLUMN], ++aerialGridPosition[i][ROW])][0][X] * (500.f * 2.f));
                break;
            default:
                //skip
                break;
        }
        aerialInstancePos[i].z = (g_mapData->gridElement[std::make_pair(aerialGridPosition[i][COLUMN], aerialGridPosition[i][ROW])][0][Z] * (500.f * 2.f)) + 200.f;
        aerialGridPossiblePath[aerialGridPosition[i][COLUMN]][aerialGridPosition[i][ROW]] = false;
        //send position to uniform
        std::string strBuffer = "offsets[" + std::to_string(i) + "]";
        const char *c_str = strBuffer.c_str();
        glUniform3fv(glGetUniformLocation(aerielShaderProgram, c_str), 1, glm::value_ptr(aerialInstancePos[i]));
    }
    //move ground instances
    glUseProgram(groundShaderProgram);
    for(unsigned int i = 0; i < groundInstanceIndex; i++) {
        bool
            pathNorth = groundGridPosition[i][COLUMN] + 1 < g_mapData->gridHeight && groundGridPossiblePath[groundGridPosition[i][COLUMN] + 1][groundGridPosition[i][ROW]],
            pathWest = groundGridPosition[i][ROW] - 1 >= 0 && groundGridPossiblePath[groundGridPosition[i][COLUMN]][groundGridPosition[i][ROW] - 1],
            pathSouth = groundGridPosition[i][COLUMN] - 1 >= 0 && groundGridPossiblePath[groundGridPosition[i][COLUMN] - 1][groundGridPosition[i][ROW]],
            pathEast = groundGridPosition[i][ROW] + 1 < g_mapData->gridWidth && groundGridPossiblePath[groundGridPosition[i][COLUMN]][groundGridPosition[i][ROW] + 1];
        //branch if the entity can't move in the same path as before
        if(!((groundLastDirection[i] == NORTH && pathNorth) || (groundLastDirection[i] == WEST && pathWest) || (groundLastDirection[i] == SOUTH && pathSouth) || (groundLastDirection[i] == EAST && pathEast))) {
            //get a random possible path
            groundLastDirection[i] = getRandomPath(pathNorth, pathWest, pathSouth, pathEast, i);
        }
        //update position according to current direction
        groundGridPossiblePath[groundGridPosition[i][COLUMN]][groundGridPosition[i][ROW]] = true;
        switch(groundLastDirection[i]) {
            case NORTH:
                groundInstancePos[i].y = (g_mapData->gridElement[std::make_pair(++groundGridPosition[i][COLUMN], groundGridPosition[i][ROW])][0][Y] * (50.f * 2.f));
                break;
            case WEST:
                groundInstancePos[i].x = (g_mapData->gridElement[std::make_pair(groundGridPosition[i][COLUMN], --groundGridPosition[i][ROW])][0][X] * (50.f * 2.f));
                break;
            case SOUTH:
                groundInstancePos[i].y = (g_mapData->gridElement[std::make_pair(--groundGridPosition[i][COLUMN], groundGridPosition[i][ROW])][0][Y] * (50.f * 2.f));
                break;
            case EAST:
                groundInstancePos[i].x = (g_mapData->gridElement[std::make_pair(groundGridPosition[i][COLUMN], ++groundGridPosition[i][ROW])][0][X] * (50.f * 2.f));
                break;
            default:
                //skip if no possible paths possible
                break;
        }
        groundInstancePos[i].z = (g_mapData->gridElement[std::make_pair(groundGridPosition[i][COLUMN], groundGridPosition[i][ROW])][0][Z] * (50.f * 2.f));
        groundGridPossiblePath[groundGridPosition[i][COLUMN]][groundGridPosition[i][ROW]] = false;
        //send position to uniform
        std::string strBuffer = "offsets[" + std::to_string(i) + "]";
        const char *c_str = strBuffer.c_str();
        glUniform3fv(glGetUniformLocation(groundShaderProgram, c_str), 1, glm::value_ptr(groundInstancePos[i]));
    }
    glUseProgram(0);
}
/**
 * @brief Finds random path.
 * 
 */
int Entity::getRandomPath(const bool pathNorth, const bool pathWest, const bool pathSouth, const bool pathEast, const int index) {
    int direction = 0;
    int indexBuffer = 0;
    //store all possible directions in an array
    std::vector<int> possiblePaths;
    //branch if path isn't opposite of current direction and there isn't a wall
    if(pathNorth) possiblePaths.push_back(NORTH);
    if(pathWest) possiblePaths.push_back(WEST);
    if(pathSouth) possiblePaths.push_back(SOUTH);
    if(pathEast) possiblePaths.push_back(EAST);
    //branch if there is at least 1 element
    if(possiblePaths.size()) {
        //branch if more then one direction is possible and shuffle the array
        if(possiblePaths.size() > 1) indexBuffer = randomIndex(0, possiblePaths.size() - 1);
        //set direction
        direction = possiblePaths[indexBuffer];
    //branch if there are no possible paths (this will pause the entity)
    } else direction = -1;
    return direction;
}
