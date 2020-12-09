/* library */
#include "header/vegetation.h"
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

Vegetation::~Vegetation() {
    glDeleteProgram(shaderProgram);
    destroyVAO(VAO);
}

Vegetation::Vegetation() {
    //fill in possible paths with true, this will be updated when getting each instance
    for(int i = 0; i < g_mapData->gridHeight; i++) {
        for(int j = 0; j < g_mapData->gridWidth; j++) {
            g_mapData->obstaclesInGridElement[i][j] = true;
        }
    }
    //construct vegetation entity
    shaderProgram = compileShader(modelVertexShader, modelFragmentShader);
	glUseProgram(shaderProgram);
    VAO = loadModel("Objects/PineTrees_LowPoly/", "pine_tree_model.obj", meshAmount);
	//change color to green;
	glUniform3fv(glGetUniformLocation(shaderProgram, "u_objectColor"), 1, glm::value_ptr(glm::vec3(0.f, 0.3f, 0.f)));
    //set projection matrix uniform
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->projectionMatrix));
    //rotate and scale entity
    glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(0.f, 0.f, 0.f));
    modelMatrix = glm::scale(glm::mat4(1.f), glm::vec3(0.01f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    //generate coordinates for each instance
    for(int i = 0; i < g_mapData->gridHeight; i += 5) {
        for(int j = 0; j < g_mapData->gridWidth; j += 5) {
            //branch if depth is between 42 and 79 (in green area)
            if(g_mapData->gridElement[std::make_pair(i, j)][0][Z] > 42 / 255.f && g_mapData->gridElement[std::make_pair(i, j)][0][2] < 79 / 255.f) {
                glm::vec3 translation;
                //multiply the value by 100 to compensate for the tree scaling(0.01f * 100.f = 1), then multiply by 2 because of the terrain scalling
                translation.x = g_mapData->gridElement[std::make_pair(i, j)][0][X] * (100.f * 2.f);
                translation.z = -g_mapData->gridElement[std::make_pair(i, j)][0][Y] * (100.f * 2.f);
                translation.y = g_mapData->gridElement[std::make_pair(i, j)][0][Z] * (100.f * 2.f);
                instancePos[instanceIndex] = translation;
                instanceIndex++;
                //remember initial position to compute coalition when moving entities
                g_mapData->obstaclesInGridElement[i][j] = false;
                //set random distance
                if(j + 10 < g_mapData->gridWidth) j += randomIndex(2, 5);
            }
        }
    }
    //send initial position to uniform
    for(unsigned int i = 0; i < instanceIndex; i++) {
        std::string buffer = "offsets[" + std::to_string(i) + "]";
        const char *uniformLoc = buffer.c_str();
        glUniform3fv(glGetUniformLocation(shaderProgram, uniformLoc), 1, glm::value_ptr(instancePos[i]));
    } 
    glUseProgram(0);
}

void Vegetation::draw() {
    //render entity
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glUniform3fv(glGetUniformLocation(shaderProgram, "u_lightDirection"), 1, glm::value_ptr(g_mapData->lightDirection));
    glUniform3fv(glGetUniformLocation(shaderProgram, "u_lightColor"), 1, glm::value_ptr(g_mapData->lightColor));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));
    glDrawArraysInstanced(GL_TRIANGLES, 6, meshAmount, instanceIndex);
    glUseProgram(0);
}
