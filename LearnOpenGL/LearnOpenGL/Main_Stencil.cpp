//#include "Main.h"
//
//// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
//// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
//// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
//#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
//#pragma comment(lib, "legacy_stdio_definitions")
//#endif
//
//using namespace std;
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//
//int Screen_H = 600;
//int Screen_W = 800;
//float lastX = Screen_W / 2;
//float lastY = Screen_H / 2;
//
//bool firstMouse = true;
//
////Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//
//// timing
//float deltaTime = 0.0f;	// time between current frame and last frame
//float lastFrame = 0.0f;
//
//unsigned int TextureFromFile(const char* imagePath) {
//
//	unsigned int textureId;
//	unsigned char* data;
//	//stbi_set_flip_vertically_on_load(true);
//	int width;
//	int height;
//	int chanels;
//	data = stbi_load(imagePath, &width, &height, &chanels, 0);
//	if (data) {
//		GLenum format;
//
//		if (chanels == 1)
//			format = GL_RED;
//		else if (chanels == 3)
//			format = GL_RGB;
//		else if (chanels == 4)
//			format = GL_RGBA;
//
//		glGenTextures(1, &textureId);
//		glBindTexture(GL_TEXTURE_2D, textureId);
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		stbi_image_free(data);
//	}
//	else {
//		std::cout << "Failed to load texture: " << imagePath << std::endl;
//		stbi_image_free(data);
//	}
//
//	return textureId;
//}
//
//
//
//void draw2Boxes(Shader2 modelShader,Box box, unsigned int cubeTexture) {
//
//	glm::mat4 model;
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, cubeTexture);
//
//	model = glm::mat4(1.0f);
//	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
//	modelShader.setMat4("model", model);
//	box.Draw(modelShader);
//
//	model = glm::mat4(1.0f);
//	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
//	modelShader.setMat4("model", model);
//	box.Draw(modelShader);
//
//}
//
//
//void draw2BoxesScaled(Shader2 modelShader, Box box, unsigned int cubeTexture) {
//	float scale = 1.1f;
//	glm::mat4 model;
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, cubeTexture);
//
//	model = glm::mat4(1.0f);
//	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
//	model = glm::scale(model, glm::vec3(scale, scale, scale));
//	modelShader.setMat4("model", model);
//	box.Draw(modelShader);
//
//	model = glm::mat4(1.0f);
//	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
//	model = glm::scale(model, glm::vec3(scale, scale, scale));
//	modelShader.setMat4("model", model);
//	box.Draw(modelShader);
//}
//
//int main() {
//
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//	GLFWwindow* window = glfwCreateWindow(Screen_W, Screen_H, "My window", NULL, NULL);
//	if (window == NULL) {
//		cout << "Failed to create GLFW window" << endl;
//		glfwTerminate();
//		return -1;
//	}
//
//	glfwMakeContextCurrent(window);
//	//set resize callback
//	//callabaks goes after the window is created and before the render loop
//	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//	glfwSetScrollCallback(window, scroll_callback);
//	glfwSetCursorPosCallback(window, mouse_callback);
//
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//		cout << "Failed to initialize GLAD" << endl;
//		return -1;
//	}
//
//	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
//	stbi_set_flip_vertically_on_load(true);
//
//	int nrAttributes;
//	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
//	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
//
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LESS);
//
//	//Shader2 depthShader("Vertex Shader\\depth_testing._vs", "Fragment Shader\\depth_testing._fs");
//
//	Shader2 modelShader("Vertex Shader\\model_shader._vs", "Fragment Shader\\ConstantTexture._fs");
//	Shader2 constantShader("Vertex Shader\\model_shader._vs", "Fragment Shader\\ConstantColor._fs");
//
//	// Setup Dear ImGui context
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//
//	// Setup Dear ImGui style
//	ImGui::StyleColorsDark();
//
//	// Setup Platform/Renderer backends
//	const char* glsl_version = "#version 130";
//	ImGui_ImplGlfw_InitForOpenGL(window, true);
//	ImGui_ImplOpenGL3_Init(glsl_version);
//
//	unsigned int cubeTexture = TextureFromFile(std::string("Images\\marble.jpg").c_str());
//	unsigned int floorTexture = TextureFromFile(std::string("Images\\metal.png").c_str());
//
//	bool show_another_window = true;
//	bool show_demo_window = true;
//
//	modelShader.use();
//	modelShader.SetInt("texture1", 0);
//
//	Box box;
//	Plane plane;
//
//	glStencilMask(0xFF);
//	glEnable(GL_STENCIL_TEST);
//
//	while (!glfwWindowShouldClose(window))
//	{
//		float currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//
//		processInput(window);
//
//		glEnable(GL_DEPTH_TEST);
//		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//
//		//Configures color used by glClear
//		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//		glStencilMask(0xFF);// tambien afecta a glclear
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//
//		//camera
//		glm::mat4 view = camera.GetViewMatrix();
//		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Screen_W / (float)Screen_H, 0.1f, 100.0f);
//		glm::mat4 model = glm::mat4(1.0f);
//
//		modelShader.use();
//		modelShader.setMat4("view", view);
//		modelShader.setMat4("projection", projection);
//
//		constantShader.use();
//		constantShader.setMat4("view", view);
//		constantShader.setMat4("projection", projection);
//
//		modelShader.use();
//
//		glStencilMask(0x00); // no actualizar stencil en suelo 
//
//		glBindTexture(GL_TEXTURE_2D, floorTexture);
//		modelShader.setMat4("model", glm::mat4(1.0f));
//		plane.Draw(modelShader);
//
//
//		glStencilFunc(GL_ALWAYS, 1, 0xFF);
//		glStencilMask(0xFF);
//
//		draw2Boxes(modelShader, box, cubeTexture);
//
//		constantShader.use();
//
//		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
//		glStencilMask(0x00);
//		glDisable(GL_DEPTH_TEST);
//
//		draw2BoxesScaled(constantShader, box, cubeTexture);
//
//		glBindVertexArray(0);
//
//		//takes the render buffer and display it on the window
//		glfwSwapBuffers(window);
//		//check event triggers
//		glfwPollEvents();
//	}
//
//	glfwTerminate();
//	return 0;
//}
//
//
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//	glViewport(0, 0, width, height);
//}
//
//void processInput(GLFWwindow* window) {
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//		glfwSetWindowShouldClose(window, true);
//	}
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
//		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
//	}
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
//		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
//	}
//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
//		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
//	}
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
//		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
//	}
//}
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
//	camera.ProcessMouseScroll(yoffset);
//}
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
//
//	if (firstMouse) {
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//	float xOffset = xpos - lastX;
//	float yOffset = lastY - ypos;
//
//	lastX = xpos;
//	lastY = ypos;
//	camera.PorcessMousemovment(xOffset, yOffset);
//}
//
//