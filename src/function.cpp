//
// Created by px on 9/26/2019.
//
#include "../include/main.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


std::string readFile(const char *filePath){
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()){
        std::cerr << "Open file failed:" << filePath<<std::endl;
        exit(-1);
    }

    std::string line ="";
    while(!fileStream.eof()){
        std::getline(fileStream, line);
        content.append(line+"\n");
    }

    fileStream.close();
    return content;
}


