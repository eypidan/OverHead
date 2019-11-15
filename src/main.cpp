#include "pch.h"

// settings
float dudv_move = 0;
int SCR_HEIGHT, SCR_WIDTH;

// ==== data init ====

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX;
float lastY;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
// lighting
glm::vec3 lightPos(0.0f, 3.0f, 0.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

using namespace nanogui;
////screen
Screen *screen = nullptr;
Color colval(1.0f, 1.0f, 1.0f, 1.0f);

int main() {
	srand(time(NULL));

	if (!glfwInit()) {
		printf("glfwInit() failed!");
		return -1;
	}
	glfwSetTime(0);

	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primary);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;
	SCR_WIDTH = mode->width;
	SCR_HEIGHT = mode->height;
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
	glfwSetMouseButtonCallback(window, mouseClickCallback);
	glfwSetKeyCallback(window,[](GLFWwindow *, int key, int scancode, int action, int mods) {	screen->keyCallbackEvent(key, scancode, action, mods);});
	glfwSetCharCallback(window,[](GLFWwindow *, unsigned int codepoint) {screen->charCallbackEvent(codepoint);}	);
	glfwSetDropCallback(window,[](GLFWwindow *, int count, const char **filenames) {screen->dropCallbackEvent(count, filenames);});
	glfwSetFramebufferSizeCallback(window,[](GLFWwindow *, int width, int height) {	screen->resizeCallbackEvent(width, height);	});
	// glad: load all OpenGL function pointers
	// ---------------------------------------

#if defined(NANOGUI_GLAD)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Could not initialize GLAD!");
	glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM
#endif

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// draw UI
	screen = new Screen();
	screen->initialize(window, true); 
	bool enabled = true;
	FormHelper *gui = new FormHelper(screen);
	nanogui::ref<nanogui::Window> nanoguiWindow = gui->addWindow(Eigen::Vector2i(0, 20), "light controler");
	gui->addGroup("lighter info");
	gui->addVariable("Color", colval)
		->setFinalCallback([](const Color &c) {
		std::cout << "ColorPicker Final Callback: ["
			<< c.r() << ", "
			<< c.g() << ", "
			<< c.b() << ", "
			<< c.w() << "]" << std::endl;
	});
	gui->addVariable("position.x", lightPos[0])->setSpinnable(true);
	gui->addVariable("position.y", lightPos[1])->setSpinnable(true);
	gui->addVariable("position.z", lightPos[2])->setSpinnable(true);

	screen->setVisible(true);
	screen->performLayout();
	nanoguiWindow->center();

	//==== data init ====
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
		//plane VAO,VBO
	unsigned int planeVBO, planeVAO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
	unsigned int woodTexture = loadTexture("resources/textures/wood.png");

	
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
	
		//the waterplane's VAO,VBO
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


	//frame buffer set up

	//set up screen vao,vbo
	unsigned int quadVAO[2], quadVBO[2];
	glGenVertexArrays(2, quadVAO);
	glGenBuffers(2, quadVBO);
	glBindVertexArray(quadVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);
	// --- --- ---
	glBindVertexArray(quadVAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices2), &quadVertices2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);

	//frame buffer 
	//buffer 1
	unsigned int framebuffer[2];
	glGenFramebuffers(2, framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[0]);
	//generate texture
	unsigned int texColorBuffer = generateAttachmentTexture(false, false);
	//Attach it to currently bound frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
	//create a render buffer
	unsigned int rbo[2];
	glGenRenderbuffers(2, rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo[0]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//attach render object to framebuffer and its attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo[0]);
	//check if framebuffer is ready
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//buffer 2
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[1]);
	//generate texture
	unsigned int texColorBuffer2 = generateAttachmentTexture(false, false);
	//Attach it to currently bound frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer2, 0);
	//create a render buffer
	glBindRenderbuffer(GL_RENDERBUFFER, rbo[1]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//attach render object to framebuffer and its attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo[1]);
	//check if framebuffer is ready
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//depth map FBO
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//==== data init ====

	// build and compile our shader zprogram
	// ------------------------------------
	Shader modelShader("./shader/modelVertexShader.glsl", "./shader/modelFragmentShader.glsl");
	Shader sunShader("./shader/sunVertex.glsl", "./shader/sunFragment.glsl");
	Shader skyboxShader("./shader/skyboxVertex.glsl","./shader/skyboxFragment.glsl");
	Shader waterShader("./shader/waterVertexShader.glsl", "./shader/waterFragmentShader.glsl");
	Shader singleColorShader("./shader/blueVertex.glsl", "./shader/blueFragment.glsl");
	Shader screenShader("./shader/screenVertexShader.glsl","./shader/screenFragmentShader.glsl");
	ShaderSet mainShaderSet(modelShader,sunShader,skyboxShader,waterShader,singleColorShader,screenShader);

    // load models
	
	//Model OrganodronCity("./models/OrganodronCity/OrganodronCity.obj");
	Model magicCube("./models/magicCube/magicCube.obj");
	//Model magicCube("./models/OrganodronCity/OrganodronCity.obj");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	
	// --- water configuration ---

	waterShader.use();
	waterShader.setVec3("lightPosition_water",lightPos);
	waterShader.setVec3("lightColor_water",lightColor);
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

    
    
	while (!glfwWindowShouldClose(window))
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		//time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);
		lightColor[0] = colval.r();lightColor[1] = colval.g();lightColor[2] = colval.b();
		/////////////////////////////////
		//right eye frame buffer render///
		/////////////////////////////////
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[0]);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // also clear the depth buffer now!
	
		mainShaderSet.modelShader->use();
		mainShaderSet.modelShader->setVec3("lightPos", lightPos);
		mainShaderSet.modelShader->setVec3("lightColor", lightColor);
		mainShaderSet.modelShader->setVec3("viewPos", camera.Position);
		//view matrix, camera transformation
		glm::mat4 view = camera.GetViewMatrix();
		mainShaderSet.modelShader->setMat4("view", view);

		//projection matrix
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		mainShaderSet.modelShader->setMat4("projection", projection);

		//model matrix
		glm::mat4 model = glm::mat4(1.0f);

		//draw the building
		//1st draw as normal
		
		
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);//enable write stencil buffer
		for (int i = 0;i < 3;i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, buildingPos[i]);
			model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
			mainShaderSet.modelShader->setMat4("model", model);
			magicCube.Draw(*mainShaderSet.modelShader); //rendering while writing into stencil buffer
			//OrganodronCity.Draw(modelShader);
		}
		//2nd draw scaled verision
		singleColorShader.use();
		singleColorShader.setMat4("view", view);
		singleColorShader.setMat4("projection", projection);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);//close write stencil buffer
	
		GLfloat scale = 1.05;
		for (int i = 0;i < 3;i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, buildingPos[i]);
			model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
			model = glm::scale(model, glm::vec3(scale, scale, scale));
			singleColorShader.setMat4("model", model);
			magicCube.Draw(singleColorShader);
			//OrganodronCity.Draw(modelShader);
		}
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
	//	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);//ttttttt

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
		glBindVertexArray(0);

		//draw light source
		sunShader.use();
		sunShader.setMat4("projection", projection);
		sunShader.setMat4("view", view);
		sunShader.setVec3("lightColor", lightColor);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		sunShader.setMat4("model", model);
		glBindVertexArray(sunVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

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

		/////////////////////////////////
		//left eye frame buffer render///
		/////////////////////////////////
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[1]);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // also clear the depth buffer now!

		modelShader.use();
		modelShader.setVec3("lightPos", lightPos);
		modelShader.setVec3("lightColor", lightColor);
		modelShader.setVec3("viewPos", camera.Position);
		//view matrix, camera transformation
		view = camera.GetViewMatrix();
		modelShader.setMat4("view", view);

		//projection matrix
		projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		modelShader.setMat4("projection", projection);

		//model matrix
		model = glm::mat4(1.0f);

		//draw the building
		//1st draw as normal

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);//enable write stencil buffer
		for (int i = 0;i < 3;i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, buildingPos[i]);
			model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
			modelShader.setMat4("model", model);
			magicCube.Draw(modelShader); //rendering while writing into stencil buffer
			//OrganodronCity.Draw(modelShader);
		}
		//2nd draw scaled verision
		singleColorShader.use();
		singleColorShader.setMat4("view", view);
		singleColorShader.setMat4("projection", projection);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);//close write stencil buffer

		scale = 1.05;
		for (int i = 0;i < 3;i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, buildingPos[i]);
			model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
			model = glm::scale(model, glm::vec3(scale, scale, scale));
			singleColorShader.setMat4("model", model);
			magicCube.Draw(singleColorShader);
			//OrganodronCity.Draw(modelShader);
		}
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);


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
		glBindVertexArray(0);

		//draw light source
		sunShader.use();
		sunShader.setMat4("projection", projection);
		sunShader.setMat4("view", view);
		sunShader.setVec3("lightColor", lightColor);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		sunShader.setMat4("model", model);
		glBindVertexArray(sunVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

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

		//=== draw quad ===
		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.use();
		glBindVertexArray(quadVAO[0]);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(quadVAO[1]);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer2);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

		////nanogui
		screen->drawContents();
		screen->drawWidgets();

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

	glDeleteFramebuffers(2, framebuffer);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

