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
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 1.f, 0.f));
    //modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.002f));
    
    glUniformMatrix4fv(glGetUniformLocation(aerielShaderProgram, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUseProgram(0);

    //construct aeriel entity
    groundShaderProgram = compileShader(modelVertexShader, modelFragmentShader);
	glUseProgram(groundShaderProgram);
    groundVAO = loadModel("Objects/MooseFemale/", "12959_Moose_Female_v1_l3.obj", groundMeshAmount); //12248_Bird_v1_L2
	//change color to brown;
	glUniform3fv(glGetUniformLocation(groundShaderProgram, "u_objectColor"), 1, glm::value_ptr(glm::vec3(0.37f, 0.34f, 0.18f)));
    //set projection matrix uniform
    glUniformMatrix4fv(glGetUniformLocation(groundShaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->projectionMatrix));
    //rotate world
    modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.4f, 0.2f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.0002f));
    
    glUniformMatrix4fv(glGetUniformLocation(groundShaderProgram, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUseProgram(0);
}

void Entity::draw() {
    glUseProgram(aerielShaderProgram);
    glBindVertexArray(aerielVAO);
    //dynamic light direction
    glm::vec3 lightDirection(sin(glfwGetTime()), -0.5f, cos(glfwGetTime()));
    glUniform3fv(glGetUniformLocation(aerielShaderProgram, "u_lightDirection"), 1, glm::value_ptr(lightDirection));
    glUniformMatrix4fv(glGetUniformLocation(aerielShaderProgram, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));
    glDrawArrays(GL_TRIANGLES, 6, aerielMeshAmount);

    glUseProgram(groundShaderProgram);
    glBindVertexArray(groundVAO);
    //dynamic light direction
    glUniform3fv(glGetUniformLocation(groundShaderProgram, "u_lightDirection"), 1, glm::value_ptr(lightDirection));
    glUniformMatrix4fv(glGetUniformLocation(groundShaderProgram, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));
    glDrawArrays(GL_TRIANGLES, 6, groundMeshAmount);

    glUseProgram(0);
}
