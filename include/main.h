z//
// Created by px on 9/26/2019.
//

#ifndef OVERHEAD_MAIN_H
#define OVERHEAD_MAIN_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <time.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./stb_image.h"
#include "./shader.h"
#include "./camera.h"
//data header
//-----------

// timing
extern float deltaTime;
extern float lastFrame ;
//camera
extern Camera camera;
extern float lastX;
extern float lastY;
extern bool firstMouse;
//-----------
//data header

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
std::string readFile(const char *filePath);


#endif //OVERHEAD_MAIN_H
