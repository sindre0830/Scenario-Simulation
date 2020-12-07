#ifndef __FUNCTION_H
#define __FUNCTION_H
/* library */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
/* functionality */
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
GLuint genObject(const std::vector<GLfloat> &arr, const int size);
std::vector<GLuint> genIndices(const int size);
GLuint compileShader(const std::string &vertexShaderSrc, const std::string &fragmentShaderSrc);
GLuint loadModel(const std::string path, const std::string file, int &size);
GLuint loadTexture(const std::string &filepath, const GLuint slot);
GLuint createVAO(const std::vector<GLfloat> &arr, const std::vector<GLuint> &arr_indices);
void destroyVAO(GLuint &VAO);
void enableDebug();
void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif
