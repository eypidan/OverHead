//
// Created by px on 9/26/2019.
//

#ifndef OVERHEAD_MAIN_H
#define OVERHEAD_MAIN_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <math.h>

class Shader {
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // use/activate the shader
    void use(){
        glUseProgram(ID);
    }
    // utility uniform functions
    void setBool(const std::string &name, bool value) const{
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const{
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const{
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
std::string readFile(const char *filePath);
#endif //OVERHEAD_MAIN_H
