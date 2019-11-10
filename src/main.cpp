#include "pch.h"
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float dudv_move = 0;
//data init
//---------
//camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
// lighting
glm::vec3 lightPos(0.0f, 3.0f, 0.0f);

using namespace nanogui;
Screen *screen = nullptr;
enum test_enum {
	Item1 = 0,
	Item2,
	Item3
};

bool bvar = true;
int ivar = 12345678;
double dvar = 3.1415926;
float fvar = (float)dvar;
std::string strval = "A string";
test_enum enumval = Item2;
Color colval(0.5f, 0.5f, 0.7f, 1.f);

int main() {
	srand(time(NULL));

	glfwInit();

	glfwSetTime(0);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Create a GLFWwindow object
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "overHead", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	
	// glad: load all OpenGL function pointers
	// ---------------------------------------

#if defined(NANOGUI_GLAD)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Could not initialize GLAD!");
	glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM
#endif

	glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// draw UI
	screen = new Screen();
	screen->initialize(window, true); 

	//data init
		// building position
	glm::vec3 buildingPos[] = {
		glm::vec3(15.0f,  0.0f, 15.0f),
		glm::vec3(-10.0f,  0.0f,  -10.0f),
		glm::vec3(-15.5f, 0.0f, 25.0f)
	};

	unsigned int cubemapTexture = loadCubemap(faces);
		//skybox VAO,VBO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
		// the sun's VAO,VBO
	unsigned int sunVAO, sunVBO;
	glGenVertexArrays(1, &sunVAO);
	glBindVertexArray(sunVAO);
	glGenBuffers(1, &sunVBO);
	glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sunVertices), sunVertices, GL_STATIC_DRAW);
	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	

		//the plane's VAO,VBO
	unsigned int waterVAO, waterVBO;
	glGenVertexArrays(1, &waterVAO);
	glBindVertexArray(waterVAO);
	glGenBuffers(1, &waterVBO);
	glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(waterPlaneVertices), waterPlaneVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,(void *)(sizeof(GLfloat) * 6 * 3));
	

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("./shader/modelVertexShader.glsl", "./shader/modelFragmentShader.glsl");
	Shader sunShader("./shader/sunVertex.glsl", "./shader/sunFragment.glsl");
	Shader skyboxShader("./shader/skyboxVertex.glsl","./shader/skyboxFragment.glsl");
	Shader waterShader("./shader/waterVertexShader.glsl", "./shader/waterFragmentShader.glsl");
	Shader singleColorShader("./shader/blueVertex.glsl", "./shader/blueFragment.glsl");
	Shader screenShader("./shader/screenVertexShader.glsl","./shader/screenFragmentShader.glsl");
	// load models
	
	//Model OrganodronCity("./models/OrganodronCity/OrganodronCity.obj");
	Model magicCube("./models/magicCube/magicCube.obj");
	
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	
	// --- water configuration ---

	waterShader.use();
	waterShader.setVec3("lightPosition_water",lightPos);
	waterShader.setVec3("lightColor_water", glm::vec3(1.0f, 1.0f, 1.0f));
	waterShader.setVec3("camera_coord", camera.Position);
	//load water texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned int obj_dudv_tex, obj_normal_tex;
	// texture 1
	// ---------
	glGenTextures(1, &obj_dudv_tex);
	glBindTexture(GL_TEXTURE_2D, obj_dudv_tex);
	unsigned char *data = stbi_load("./textures/dudv2.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// ---------
	glGenTextures(1, &obj_normal_tex);
	glBindTexture(GL_TEXTURE_2D, obj_normal_tex);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("./textures/normalMap2.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	waterShader.setInt("tex_refraction", 2);
	waterShader.setInt("tex_reflection",3);
	waterShader.setInt("tex_dudv",4 );
	waterShader.setInt("tex_normal", 5);
	
	
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, obj_dudv_tex);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, obj_normal_tex);

	// --- water configuration END ---


	// --- skybox configuration --- 
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	// --- skybox configuration END --- 
	
	// render loop
	// -----------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


	while (!glfwWindowShouldClose(window))
	{

		//time logic
		//------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(97 / 256.f, 175 / 256.f, 239 / 256.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // also clear the depth buffer now!

		ourShader.use();
		ourShader.setVec3("lightPos", lightPos);
		ourShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		ourShader.setVec3("viewPos", camera.Position);
		//view matrix, camera transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		//projection matrix
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		//model matrix
		glm::mat4 model = glm::mat4(1.0f);

		//draw the building
		//1st draw as normal
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);//enable write stencil buffer
		for (int i = 0;i < 3;i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, buildingPos[i]);
			model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
			ourShader.setMat4("model", model);
			magicCube.Draw(ourShader); //rendering while writing into stencil buffer
			//OrganodronCity.Draw(ourShader);
		}
		//2nd draw scaled verision
		singleColorShader.use();
		singleColorShader.setMat4("view", view);
		singleColorShader.setMat4("projection", projection);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);//close write stencil buffer
		//glDisable(GL_DEPTH_TEST);
		GLfloat scale = 1.05;
		for (int i = 0;i < 3;i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, buildingPos[i]);
			model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
			model = glm::scale(model, glm::vec3(scale, scale, scale));
			singleColorShader.setMat4("model", model);
			magicCube.Draw(singleColorShader);
			//OrganodronCity.Draw(ourShader);
		}
		glStencilMask(0xFF);
		//glEnable(GL_DEPTH_TEST);


		//draw water plane

		waterShader.use();
		waterShader.setMat4("projection", projection);
		waterShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(5.0f));
		waterShader.setMat4("model", model);
		dudv_move += 0.0005f; // speed
		dudv_move = fmod(dudv_move, 1.0f);
		waterShader.setFloat("dudv_move", dudv_move);

		glBindVertexArray(waterVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		//draw light source
		sunShader.use();
		sunShader.setMat4("projection", projection);
		sunShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		sunShader.setMat4("model", model);
		glBindVertexArray(sunVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//draw skybox last

		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix, but keep rotation	
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cubeGL_LESS
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
		

		// === render another window  === 
		

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVAO);

	glDeleteVertexArrays(1, &waterVAO);
	glDeleteBuffers(1, &waterVAO);

	glDeleteVertexArrays(1, &sunVAO);
	glDeleteBuffers(1, &sunVAO);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

