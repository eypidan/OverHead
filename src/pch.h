#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <time.h>
#include <vector>
#include <string>


#define NANOGUI_GLAD
#if defined(NANOGUI_GLAD)
    #if defined(NANOGUI_SHARED) && !defined(GLAD_GLAPI_EXPORT)
        #define GLAD_GLAPI_EXPORT
    #endif

    #include <glad/glad.h>
#else
    #if defined(__APPLE__)
        #define GLFW_INCLUDE_GLCOREARB
    #else
        #define GL_GLEXT_PROTOTYPES
    #endif
#endif

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//nanoGUi
#include <nanogui/nanogui.h>

#define STB_IMAGE_IMPLEMENTATION 
#define STB_IMAGE_STATIC
#include "../include/stb_image.h"
#include "../include/shader.h"
#include "../include/camera.h"
#include "../include/mesh.h"
#include "../include/model.h"

//data header
//-----------

extern int SCR_WIDTH;
extern int SCR_HEIGHT;

// timing
extern float deltaTime;
extern float lastFrame;

//camera
extern Camera camera;
extern Camera cameraLeft;
extern Camera cameraRight;
extern float lastX;
extern float lastY;
extern bool firstMouse;

// initialization
extern float waterPlaneVertices[30];
extern float sunVertices[216];
extern float skyboxVertices[108];
extern float quadVertices[24];
extern float planeVertices[48];
extern float quadVertices2[24];
extern nanogui::Screen *screen;
extern vector<std::string> faces;
//light
extern glm::vec3 lightPos;
extern glm::vec3 lightColor;
extern Model magicCube;

//picking
extern double mouseX, mouseY;
extern glm::vec3 buildingPos[3];
extern int pickedID;
//-----------
//data header

//vao struct
class vaoSet {
public:
	int sunVAO;
	int waterVAO;
	int planeVAO;
	int skyboxVAO;
	vaoSet(int sunVAO,int waterVAO,int planeVAO, int skyboxVAO){
		this->sunVAO = sunVAO;
		this->waterVAO = waterVAO;
		this->planeVAO = planeVAO;
		this->skyboxVAO = skyboxVAO;
	}
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouseClickCallback(GLFWwindow* window,int button, int action, int modifiers);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
std::string readFile(const char *filePath);
unsigned int loadCubemap(vector<std::string> faces);
unsigned int loadTexture(const char *path);
unsigned int generateAttachmentTexture(GLboolean depth, GLboolean stencil);

//render function
void renderCube(Shader modelShader, Camera currentCamera, vaoSet inputVAO,Model *magicmagicCubeCube);
void renderSide(Shader singleColorShader, Camera currentCamera, Model *magicCube);
void renderSkybox(Shader skyboxShader,unsigned int skyboxVAO,Camera currentCamera);
void renderSun(Shader sunShader, unsigned int sunVAO, Camera currentCamera);
void renderQuad();
#endif //PCH_H
