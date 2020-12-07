/* library */
#include "header/terrain.h"
#include "shader/terrain.h"
#include "dictionary.h"
#include "camera.h"
#include "functionality.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
/* global data */
extern Camera *g_camera;

Terrain::~Terrain() {
    glDeleteProgram(shaderProgram);
    destroyVAO(VAO);
}

Terrain::Terrain() {
    //compile scoreboard shader
    shaderProgram = compileShader(terrainVertexShader, terrainFragmentShader);
    //create VAO
	std::vector<GLfloat> arr = genCoordinates();
    VAO = genObject(arr, 1);
    //specify the layout of the vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const void*)(2 * sizeof(GLfloat)));
    //set projection matrix uniform
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->projectionMatrix));
    //rotate world
    glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUseProgram(0);
}
/**
 * @brief Draw object by activating the shader program and binding the VAO and uniforms to the current rendering state.
 * 
 */
void Terrain::draw() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glUniform1i(glGetUniformLocation(shaderProgram, "u_texture"), TERRAIN_TEXTURE);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
    glUseProgram(0);
}

std::vector<GLfloat> Terrain::genCoordinates() {
    std::vector<GLfloat> arr = {
        //top left grid and texture coordinate
        -1.f, 1.f,	
        0.f, 1.f,
        //bottom left grid and texture coordinate
        -1.f, -1.f, 
        0.f, 0.f,
        //bottom right grid and texture coordinate
        1.f, -1.f, 
        1.f, 0.f,
        //top right grid and texture coordinate
        1.f, 1.f,	
        1.f, 1.f
    };
    return arr;
}