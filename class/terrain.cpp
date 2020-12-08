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
//extern std::vector<std::vector<int>> pixelDepth;
//extern std::map<std::pair<int, int>, std::vector<std::vector<float>>> gridElement;

Terrain::~Terrain() {
    glDeleteProgram(shaderProgram);
    destroyVAO(VAO);
}

Terrain::Terrain() {
    int gridWidth = 200, gridHeight = 200;

    //std::map<std::pair<int, int>, std::vector<std::vector<float>>> gridElement;


    //compile scoreboard shader
    shaderProgram = compileShader(terrainVertexShader, terrainFragmentShader);
    //create VAO
    std::vector<GLfloat> arr;
    for(int i = 0; i < gridHeight; i++) {
        for(int j = 0; j < gridWidth; j++) {
            for(int k = 0; k < 4; k++) {
                for(int n = 0; n < 5; n++) {
                    arr.push_back(g_mapData->gridElement[std::make_pair(i, j)][k][n]);
                }
                meshAmount++;
            }
        }
    }
    /*const float elementWidth = 1 / (float)(gridWidth), textureWidth = 1 / ((float)(gridWidth) / 2.f);
    int pixelIndexRow = 0, pixelIndexCol = 0;
    for(float yPos = -1.f, yTex = 0.f; yPos <= 1.f; yPos += elementWidth, yTex += textureWidth) {
        for(float xPos = -1.f, xTex = 0.f; xPos <= 1.f; xPos += elementWidth, xTex += textureWidth) {
            arr.insert(arr.end(), {
                //top left
                xPos, yPos + elementWidth, 10.f / (float)(pixelDepth[pixelIndexCol][pixelIndexRow]),
                xTex, yTex + textureWidth,
                //bottom left
                xPos, yPos, 10.f / (float)(pixelDepth[pixelIndexCol][pixelIndexRow]),
                xTex, yTex,
                //bottom right
                xPos + elementWidth, yPos, 10.f / (float)(pixelDepth[pixelIndexCol + 1][pixelIndexRow]),
                xTex + textureWidth, yTex,
                //top right
                xPos + elementWidth, yPos + elementWidth, 10.f / (float)(pixelDepth[pixelIndexCol + 1][pixelIndexRow + 1]),
                xTex + textureWidth, yTex + textureWidth
            });
            meshAmount++;
            if(pixelIndexRow + 1 < 541) pixelIndexRow++;
        }
        pixelIndexRow = 0;
        if(pixelIndexCol + 1 < 541) pixelIndexCol++;
    }*/
    std::cout << meshAmount << std::endl;
    VAO = genObject(arr, meshAmount);
    //specify the layout of the vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*)(3 * sizeof(GLfloat)));
    //set projection matrix uniform
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->projectionMatrix));
    //rotate world
    glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
    //modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(10.f));
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
    glDrawElements(GL_TRIANGLES, (6 * meshAmount), GL_UNSIGNED_INT, (const void*)0);
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
