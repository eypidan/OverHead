//
// Created by px on 9/26/2019.
//

#ifndef OVERHEAD_MAIN_H
#define OVERHEAD_MAIN_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
std::string readFile(const char *filePath);
#endif //OVERHEAD_MAIN_H
