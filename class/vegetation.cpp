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
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.4f, 0.f));
    //modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));
    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUseProgram(0);
}

void Vegetation::draw() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    //dynamic light direction
    glm::vec3 lightDirection(sin(glfwGetTime()), -0.5f, cos(glfwGetTime()));
    glUniform3fv(glGetUniformLocation(shaderProgram, "u_lightDirection"), 1, glm::value_ptr(lightDirection));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));
    glDrawArrays(GL_TRIANGLES, 6, meshAmount);
}
