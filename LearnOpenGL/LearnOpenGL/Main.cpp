#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <fstream>
#include <sstream> 
#include <string> 
#include "Shader.h"
#include "Shader2.h"
#include "VertexShader.h"
#include "FragmentShader.h"
#include "OGLProgram.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Cube.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Model.h"
#include <filesystem>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


int Screen_H = 600;
int Screen_W = 800;
float lastX = Screen_W / 2;
float lastY = Screen_H / 2;

bool firstMouse = true;

//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(Screen_W, Screen_H, "My window", NULL, NULL);
	if (window == NULL) {
		cout<<"Failed to create GLFW window"<<endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//set resize callback
	//callabaks goes after the window is created and before the render loop
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	glEnable(GL_DEPTH_TEST);

	Shader2 modelShader("Vertex Shader\\model_shader._vs", "Fragment Shader\\model_shader._fs");
	Shader2 lightShader("Vertex Shader\\cube_lighting._vs", "Fragment Shader\\light_lighting._fs");

	string pathModel = "models\\backpack\\backpack.obj";
	Model ourModel(pathModel.c_str());

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	const char* glsl_version = "#version 130";
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -1.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};




	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticesNormalTexture), cubeVerticesNormalTexture, GL_STATIC_DRAW);

	//define attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//shader
	modelShader.use();
	modelShader.SetVec3("dirLight.direction", camera.GetViewMatrix() * glm::vec4(glm::vec3(-0.2f, -1.0f, -0.3f),0.0f));
	modelShader.SetVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	modelShader.SetVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
	modelShader.SetVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

	// point light 1
	modelShader.SetVec3("pointLights[0].position", camera.GetViewMatrix()* glm::vec4(pointLightPositions[0], 1.0f));
	modelShader.SetVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	modelShader.SetVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	modelShader.SetVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	modelShader.SetFloat("pointLights[0].constant", 1.0f);
	modelShader.SetFloat("pointLights[0].linear", 0.09);
	modelShader.SetFloat("pointLights[0].quadratic", 0.032);
	// point light 2
	modelShader.SetVec3("pointLights[1].position", camera.GetViewMatrix()* glm::vec4(pointLightPositions[1],1.0f));
	modelShader.SetVec3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	modelShader.SetVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	modelShader.SetVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	modelShader.SetFloat("pointLights[1].constant", 1.0f);
	modelShader.SetFloat("pointLights[1].linear", 0.09);
	modelShader.SetFloat("pointLights[1].quadratic", 0.032);
	// point light 3
	modelShader.SetVec3("pointLights[2].position", camera.GetViewMatrix()* glm::vec4(pointLightPositions[2], 1.0f));
	modelShader.SetVec3("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	modelShader.SetVec3("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	modelShader.SetVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	modelShader.SetFloat("pointLights[2].constant", 1.0f);
	modelShader.SetFloat("pointLights[2].linear", 0.09);
	modelShader.SetFloat("pointLights[2].quadratic", 0.032);
	// point light 4
	modelShader.SetVec3("pointLights[3].position", camera.GetViewMatrix()* glm::vec4(pointLightPositions[3], 1.0f));
	modelShader.SetVec3("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	modelShader.SetVec3("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	modelShader.SetVec3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	modelShader.SetFloat("pointLights[3].constant", 1.0f);
	modelShader.SetFloat("pointLights[3].linear", 0.09);
	modelShader.SetFloat("pointLights[3].quadratic", 0.032);


	modelShader.SetVec3("spotLight.position", camera.GetViewMatrix()* glm::vec4(camera.CameraPosition(), 1.0f));
	modelShader.SetVec3("spotLight.direction", camera.GetViewMatrix()* glm::vec4(camera.CameraDirection(), 0.0f));
	modelShader.SetVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	modelShader.SetVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	modelShader.SetVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	modelShader.SetFloat("spotLight.constant", 1.0f);
	modelShader.SetFloat("spotLight.linear", 0.09);
	modelShader.SetFloat("spotLight.quadratic", 0.032);
	modelShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	modelShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


	bool show_another_window = true;
	bool show_demo_window = true;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//Configures color used by glClear
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


		//camera
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Screen_W / (float)Screen_H, 0.1f, 100.0f);


		//lights
		lightShader.use();
		glUniformMatrix4fv(lightShader.GetUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(lightShader.GetUniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(lightVAO);
		for (unsigned int i = 0; i < 4; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(lightShader.GetUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}



		modelShader.use();
		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		glUniformMatrix4fv(modelShader.GetUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(modelShader.GetUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelShader.GetUniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
		
		modelShader.SetVec3("spotLight.position", camera.GetViewMatrix() * glm::vec4(camera.CameraPosition(), 1.0f));
		modelShader.SetVec3("spotLight.direction", camera.GetViewMatrix() * glm::vec4(camera.CameraDirection(), 0.0f));

		modelShader.SetVec3("dirLight.direction", camera.GetViewMatrix() * glm::vec4(glm::vec3(-0.2f, -1.0f, -0.3f), 0.0f));
		modelShader.SetVec3("pointLights[0].position", camera.GetViewMatrix() * glm::vec4(pointLightPositions[0], 1.0f));
		modelShader.SetVec3("pointLights[1].position", camera.GetViewMatrix() * glm::vec4(pointLightPositions[1], 1.0f));
		modelShader.SetVec3("pointLights[2].position", camera.GetViewMatrix() * glm::vec4(pointLightPositions[2], 1.0f));
		modelShader.SetVec3("pointLights[3].position", camera.GetViewMatrix() * glm::vec4(pointLightPositions[3], 1.0f));
		

		//draw model
		ourModel.Draw(modelShader);


		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow(&show_demo_window);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		//takes the render buffer and display it on the window
		glfwSwapBuffers(window);
		//check event triggers
		glfwPollEvents();
		glBindVertexArray(0);
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xOffset = xpos - lastX;
	float yOffset = lastY- ypos;

	lastX = xpos;
	lastY = ypos;
	camera.PorcessMousemovment(xOffset, yOffset);
}