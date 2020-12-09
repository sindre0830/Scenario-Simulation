/* library */
#include "header/terrain.h"
#include "shader/terrain.h"
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

Terrain::~Terrain() {
    glDeleteProgram(shaderProgram);
    glDeleteProgram(waterShaderProgram);
    destroyVAO(VAO);
    destroyVAO(waterVAO);
}

Terrain::Terrain() {
    //construct terrain map
    shaderProgram = compileShader(terrainVertexShader, terrainFragmentShader);
    //create VAO
    std::vector<GLfloat> arr;
    for(int i = 0; i < g_mapData->gridHeight; i++) {
        for(int j = 0; j < g_mapData->gridWidth; j++) {
            for(int k = 0; k < 4; k++) {
                for(int n = 0; n < 8; n++) {
                    arr.push_back(g_mapData->gridElement[std::make_pair(i, j)][k][n]);
                }
                meshAmount++;
            }
        }
    }
    VAO = genObject(arr, meshAmount);
    //specify the layout of the vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*)(5 * sizeof(GLfloat)));
    //set projection matrix uniform
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->projectionMatrix));
    //rotate and scale world
    glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2.f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    //construct water texture
    waterShaderProgram = compileShader(terrainVertexShader, terrainFragmentShader);
    //create VAO
    arr = genCoordinates();
    waterVAO = genObject(arr, 1);
    //specify the layout of the vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*)(5 * sizeof(GLfloat)));
    //set projection matrix uniform
    glUniformMatrix4fv(glGetUniformLocation(waterShaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->projectionMatrix));
    //rotate and scale world
    glUniformMatrix4fv(glGetUniformLocation(waterShaderProgram, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUseProgram(0);
}
/**
 * @brief Draw object by activating the shader program and binding the VAO and uniforms to the current rendering state.
 * 
 */
void Terrain::draw() {
    //render terrain map
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glUniform3fv(glGetUniformLocation(shaderProgram, "u_lightDirection"), 1, glm::value_ptr(g_mapData->lightDirection));
    glUniform3fv(glGetUniformLocation(shaderProgram, "u_lightColor"), 1, glm::value_ptr(g_mapData->lightColor));
    glUniform1i(glGetUniformLocation(shaderProgram, "u_texture"), TERRAIN_TEXTURE);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));
    glDrawElements(GL_TRIANGLES, (6 * meshAmount), GL_UNSIGNED_INT, (const void*)0);
    //render water texture
    glUseProgram(waterShaderProgram);
    glBindVertexArray(waterVAO);
    glUniform3fv(glGetUniformLocation(waterShaderProgram, "u_lightDirection"), 1, glm::value_ptr(g_mapData->lightDirection));
    glUniform3fv(glGetUniformLocation(waterShaderProgram, "u_lightColor"), 1, glm::value_ptr(g_mapData->lightColor));
    glUniform1i(glGetUniformLocation(waterShaderProgram, "u_texture"), WATER_TEXTURE);
    glUniformMatrix4fv(glGetUniformLocation(waterShaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(waterShaderProgram, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
    glUseProgram(0);
}

std::vector<GLfloat> Terrain::genCoordinates() {
    //creates full scale 2D rectangle
    std::vector<GLfloat> arr = {
        //top left grid, texture and normal coordinates
        -1.f, 1.f, 30.f / 255.f,
        0.f, 1.f,
        0.5f, 0.5f, 0.5f,
        //bottom left grid, texture and normal coordinates
        -1.f, -1.f, 30.f / 255.f,
        0.f, 0.f,
        0.5f, 0.5f, 0.5f,
        //bottom right grid, texture and normal coordinates
        1.f, -1.f, 30.f / 255.f,
        1.f, 0.f,
        0.5f, 0.5f, 0.5f,
        //top right grid, texture and normal coordinates
        1.f, 1.f, 30.f / 255.f,
        1.f, 1.f,
        0.5f, 0.5f, 0.5f
    };
    return arr;
}
