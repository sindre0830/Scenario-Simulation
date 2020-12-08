/* library */
#include "header/entity.h"
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

Entity::~Entity() {
    glDeleteProgram(aerielShaderProgram);
    glDeleteProgram(groundShaderProgram);
    destroyVAO(aerielVAO);
    destroyVAO(groundVAO);
}

Entity::Entity() {
    //construct aeriel entity
    aerielShaderProgram = compileShader(modelVertexShader, modelFragmentShader);
	glUseProgram(aerielShaderProgram);
    aerielVAO = loadModel("Objects/Eagle/", "Eagle02.obj", aerielMeshAmount); //12248_Bird_v1_L2
	//change color to brown;
	glUniform3fv(glGetUniformLocation(aerielShaderProgram, "u_objectColor"), 1, glm::value_ptr(glm::vec3(0.37f, 0.34f, 0.18f)));
    //set projection matrix uniform
    glUniformMatrix4fv(glGetUniformLocation(aerielShaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->projectionMatrix));
    //rotate world
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.002f));
    glUniformMatrix4fv(glGetUniformLocation(aerielShaderProgram, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::vec3 translations[42];
    int index = 0;
    for(int i = 0; i < g_mapData->gridHeight; i += 20) {
        for(int j = 0; j < g_mapData->gridWidth; j += 20) {
            if(g_mapData->gridElement[std::make_pair(i, j)][0][2] > 75 / 255.f) {
                /*for(int n = 0; n < 3; n++) {
                    arrTestBuffer.push_back(g_mapData->gridElement[std::make_pair(i, j)][0][n]);
                }*/
                //arrTest.push_back(arrTestBuffer);
                glm::vec3 translation;
                //multiply the value by 500 to compensate for the bird scaling(0.002f * 500.f = 1), then multiply by 2 because of the terrain scalling(2.f)
                translation.x = g_mapData->gridElement[std::make_pair(i, j)][0][0] * (500.f * 2.f);
                translation.y = g_mapData->gridElement[std::make_pair(i, j)][0][1] * (500.f * 2.f);
                translation.z = g_mapData->gridElement[std::make_pair(i, j)][0][2] * (500.f * 2.f) + 200.f;
                translations[index] = translation;
                index++;
            }
        }
    }
    //std::cout << index << std::endl;

    for(unsigned int i = 0; i < 42; i++) {
        std::string strBuffer = "offsets[" + std::to_string(i) + "]";
        const char *c_str = strBuffer.c_str();
        glUniform3fv(glGetUniformLocation(aerielShaderProgram, c_str), 1, glm::value_ptr(translations[i]));
    } 

    //construct ground entity
    groundShaderProgram = compileShader(modelVertexShader, modelFragmentShader);
	glUseProgram(groundShaderProgram);
    groundVAO = loadModel("Objects/Deer2/", "deer.obj", groundMeshAmount); //12248_Bird_v1_L2
	//change color to brown;
	glUniform3fv(glGetUniformLocation(groundShaderProgram, "u_objectColor"), 1, glm::value_ptr(glm::vec3(0.37f, 0.34f, 0.18f)));
    //set projection matrix uniform
    glUniformMatrix4fv(glGetUniformLocation(groundShaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->projectionMatrix));
    //rotate world
    modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02f));
    glUniformMatrix4fv(glGetUniformLocation(groundShaderProgram, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

    translations[18];
    index = 0;
    for(int i = 0; i < g_mapData->gridHeight; i += 20) {
        for(int j = 0; j < g_mapData->gridWidth; j += 20) {
            if(g_mapData->gridElement[std::make_pair(i, j)][0][2] > 40 / 255.f && g_mapData->gridElement[std::make_pair(i, j)][0][2] < 60 / 255.f) {
                /*for(int n = 0; n < 3; n++) {
                    arrTestBuffer.push_back(g_mapData->gridElement[std::make_pair(i, j)][0][n]);
                }*/
                //arrTest.push_back(arrTestBuffer);
                glm::vec3 translation;
                //multiply the value by 50 to compensate for the moose scaling(0.02f * 50.f = 1), then multiply by 2 because of the terrain scalling(2.f)
                translation.x = g_mapData->gridElement[std::make_pair(i, j)][0][0] * (50.f * 2.f);
                translation.y = g_mapData->gridElement[std::make_pair(i, j)][0][1] * (50.f * 2.f);
                translation.z = g_mapData->gridElement[std::make_pair(i, j)][0][2] * (50.f * 2.f);
                translations[index] = translation;
                index++;
            }
        }
    }
    //std::cout << index << std::endl;

    for(unsigned int i = 0; i < 18; i++) {
        std::string strBuffer = "offsets[" + std::to_string(i) + "]";
        const char *c_str = strBuffer.c_str();
        glUniform3fv(glGetUniformLocation(groundShaderProgram, c_str), 1, glm::value_ptr(translations[i]));
    }

    glUseProgram(0);
}

void Entity::draw() {
    glUseProgram(aerielShaderProgram);
    glBindVertexArray(aerielVAO);
    //dynamic light direction
    glm::vec3 lightDirection(sin(glfwGetTime()), 3.f, cos(glfwGetTime()));
    glUniform3fv(glGetUniformLocation(aerielShaderProgram, "u_lightDirection"), 1, glm::value_ptr(lightDirection));
    glUniformMatrix4fv(glGetUniformLocation(aerielShaderProgram, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));
    glDrawArraysInstanced(GL_TRIANGLES, 6, aerielMeshAmount, 42);
    //glDrawArrays(GL_TRIANGLES, 6, aerielMeshAmount);

    glUseProgram(groundShaderProgram);
    glBindVertexArray(groundVAO);
    //dynamic light direction
    glUniform3fv(glGetUniformLocation(groundShaderProgram, "u_lightDirection"), 1, glm::value_ptr(lightDirection));
    glUniformMatrix4fv(glGetUniformLocation(groundShaderProgram, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));
    glDrawArraysInstanced(GL_TRIANGLES, 6, aerielMeshAmount, 18);
    //glDrawArrays(GL_TRIANGLES, 6, groundMeshAmount);

    glUseProgram(0);
}
