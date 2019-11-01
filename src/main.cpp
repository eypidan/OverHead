//opengl version 4.5
//
#include "pch.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);


int main() {
	srand(time(NULL));
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	//data init
		// building position
	glm::vec3 buildingPos[] = {
		glm::vec3(-10.0f,  0.0f,  -10.0f),
		glm::vec3(15.0f,  0.0f, 15.0f),
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

		//the plane's VAO,VBO
	unsigned int waterVAO, waterVBO;
	glGenVertexArrays(1, &waterVAO);
	glGenBuffers(1, &waterVBO);
	glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(waterPlaneVertices), waterPlaneVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("./shader/vertexShader.glsl", "./shader/fragmentShader.glsl");
	Shader sunShader("./shader/sunVertex.glsl", "./shader/sunFragment.glsl");
	Shader skyboxShader("./shader/skyboxVertex.glsl","./shader/skyboxFragment.glsl");
	Shader waterShader("./shader/waterVertexShader.glsl", "./shader/waterFragmentShader.glsl");
	// load models
	
	//Model OrganodronCity("./models/OrganodronCity/OrganodronCity.obj");

	
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	

	//skybox configuration
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	
	// render loop
	// -----------
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
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		ourShader.use();
		ourShader.setVec3("lightPos", lightPos);
		ourShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		ourShader.setVec3("viewPos", camera.Position);
		//view matrix, camera transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		//projection matrix
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		//model matrix
		glm::mat4 model = glm::mat4(1.0f);

		//draw the building
		for (int i = 0;i < 3;i++) {
			//buildingPos
			model = glm::mat4(1.0f);
			model = glm::translate(model, buildingPos[i]);
			model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
			ourShader.setMat4("model", model);
			//nanosuit.Draw(ourShader);
			//OrganodronCity.Draw(ourShader);
		}
		
		//draw water plane
		waterShader.use();
		waterShader.setMat4("projection", projection);
		waterShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(5.0f));
		waterShader.setMat4("model", model);
		glBindVertexArray(waterVAO);
		glDrawArrays(GL_TRIANGLES, 0, 4);


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

		//glDepthFunc(GL_LEQUAL);
		//skyboxShader.use();
		//view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		//skyboxShader.setMat4("view", view);
		//skyboxShader.setMat4("projection", projection);
		//// skybox cube
		//glBindVertexArray(skyboxVAO);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);
		//glDepthFunc(GL_LESS); // set depth function back to default
		
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

