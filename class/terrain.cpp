/* library */
#include "header/terrain.h"
#include "shader/terrain.h"
#include "dictionary.h"
#include "functionality.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

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