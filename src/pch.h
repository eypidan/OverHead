// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <time.h>
#include <vector>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//nanoGUI
#include <nanogui/nanogui.h>


#define STB_IMAGE_IMPLEMENTATION 
#define STB_IMAGE_STATIC
#include "../include/stb_image.h"
#include "../include/shader.h"
#include "../include/camera.h"
#include "../include/mesh.h"
#include "../include/model.h"
#include "../include/water.h"
//data header
//-----------

// timing
extern float deltaTime;
extern float lastFrame;

//camera
extern Camera camera;
extern float lastX;
extern float lastY;
extern bool firstMouse;

// initialization
extern float waterPlaneVertices[30];
extern float sunVertices[216];
extern float skyboxVertices[108];
extern vector<std::string> faces;

//-----------
//data header

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
std::string readFile(const char *filePath);
unsigned int loadCubemap(vector<std::string> faces);

#endif //PCH_H
