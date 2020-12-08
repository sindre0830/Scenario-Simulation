/* library */
#include "header/vegetation.h"
#include "shader/modelShader.h"
#include "dictionary.h"
#include "camera.h"
#include "mapData.h"
#include "functionality.h"
#include <glm/glm.hpp>
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
    shaderProgram = compileShader(modelVertexShader, modelFragmentShader);
	glUseProgram(shaderProgram);
    VAO = loadModel("Objects/PineTrees_LowPoly/", "pine_tree_model.obj", meshAmount);
	//change color to green;
	glUniform3fv(glGetUniformLocation(shaderProgram, "u_objectColor"), 1, glm::value_ptr(glm::vec3(0.f, 0.4f, 0.f)));
    //set projection matrix uniform
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->projectionMatrix));
    //rotate world
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
    //modelMatrix = glm::rotate(modelMatrix, glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));
    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

    std::vector<std::vector<GLfloat>> arrTest;
    std::vector<GLfloat> arrTestBuffer;
    glm::vec3 translations[600];
    int index = 0;
    for(int i = 0; i < g_mapData->gridHeight; i += 5) {
        for(int j = 0; j < g_mapData->gridWidth; j += 5) {
            if(g_mapData->gridElement[std::make_pair(i, j)][0][2] > 42 / 255.f && g_mapData->gridElement[std::make_pair(i, j)][0][2] < 79 / 255.f) {
                /*for(int n = 0; n < 3; n++) {
                    arrTestBuffer.push_back(g_mapData->gridElement[std::make_pair(i, j)][0][n]);
                }*/
                //arrTest.push_back(arrTestBuffer);
                glm::vec3 translation;
                //multiply the value by 100 to compensate for the tree scaling(0.01f * 100.f = 1), then multiply by 2 because of the terrain scalling
                translation.x = g_mapData->gridElement[std::make_pair(i, j)][0][0] * (100.f * 2.f);
                translation.y = g_mapData->gridElement[std::make_pair(i, j)][0][1] * (100.f * 2.f);
                translation.z = g_mapData->gridElement[std::make_pair(i, j)][0][2] * (100.f * 2.f);
                translations[index] = translation;
                index++;
            }
        }
    }
    //std::cout << index << std::endl;

    /*glm::vec3 translation;
    translation.x = g_mapData->gridElement[std::make_pair(0, 0)][0][0] * 200.f;
    translation.y = g_mapData->gridElement[std::make_pair(0, 0)][0][1] * 20.f;
    translation.z = g_mapData->gridElement[std::make_pair(0, 0)][0][2] * 200.f;
    translations[0] = translation;
    
    translation.x = g_mapData->gridElement[std::make_pair(199, 199)][0][0] * 200.f;
    translation.y = g_mapData->gridElement[std::make_pair(199, 199)][0][1] * 20.f;
    translation.z = g_mapData->gridElement[std::make_pair(199, 199)][0][2] * 200.f;
    translations[1] = translation;*/

    for(unsigned int i = 0; i < 600; i++) {
        std::string strBuffer = "offsets[" + std::to_string(i) + "]";
        const char *c_str = strBuffer.c_str();
        glUniform3fv(glGetUniformLocation(shaderProgram, c_str), 1, glm::value_ptr(translations[i]));
    } 

    glUseProgram(0);
}

void Vegetation::draw() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    //dynamic light direction
    glm::vec3 lightDirection(sin(glfwGetTime()), 3.f, cos(glfwGetTime()));
    glUniform3fv(glGetUniformLocation(shaderProgram, "u_lightDirection"), 1, glm::value_ptr(lightDirection));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));
    glDrawArraysInstanced(GL_TRIANGLES, 6, meshAmount, 600);
    glUseProgram(0);
}
