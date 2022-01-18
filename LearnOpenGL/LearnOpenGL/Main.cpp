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

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


using namespace std;

bool firstMouse;

int Screen_H = 600;
int Screen_W = 800;

float lastX = Screen_W / 2;
float lastY = Screen_H / 2;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main() {
	stbi_set_flip_vertically_on_load(true);
	const char* glsl_version = "#version 130";
	glm::vec3 coral(1.0f, 0.5f, 0.31f);
	firstMouse = true;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(Screen_W, Screen_H, "My window", NULL, NULL);
	if (window == NULL) {
		cout<<"Failed to create GLFW window"<<endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	// tell open gl the size of the window
	glViewport(0, 0, Screen_W, Screen_H);
	//set resize callback
	//callabaks goes after the window is created and before the render loop
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	//triangle 
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f
	};

	float texCoords[] = {
	0.0f, 0.0f,  // lower-left corner  
	1.0f, 0.0f,  // lower-right corner
	0.5f, 1.0f   // top-center corner
	};

	float vertices2[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -1.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	unsigned int indices[] = { // note that we start from 0!
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
	};

	//texture config 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// bind VAO
	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
	//GL_STATIC_DRAW : the data is set only once and used many times.
	//GL_DYNAMIC_DRAW : the data is changed a lot and used many times.
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticesNormalTexture), cubeVerticesNormalTexture, GL_STATIC_DRAW);

	//define attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// add buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//define attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//shader
	Shader2 cubeShader("Vertex Shader\\map_lighting._vs", "Fragment Shader\\MultiLight._fs");
	Shader2 lightShader("Vertex Shader\\cube_lighting._vs", "Fragment Shader\\light_lighting._fs");

	//set cube color
	cubeShader.use();
	Texture diffuse("Images\\container2.png", 0);
	Texture specular("Images\\container2_specular.png", 1);
	//cubeShader.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	//cubeShader.SetVec3("material.ambient", glm::vec3(0.19225f, 0.19225f, 0.19225f));
	//cubeShader.SetVec3("material.diffuse", glm::vec3(0.50754f, 0.50754f, 0.50754f));
	cubeShader.SetInt("material.specular", specular.Bind_);
	cubeShader.SetFloat("material.shininess", 128.0f * 0.4f);
	cubeShader.SetInt("material.ambient", diffuse.Bind_);




	cubeShader.SetVec3("dirLight.direction", camera.GetViewMatrix() * glm::vec4(glm::vec3(-0.2f, -1.0f, -0.3f),0.0f));
	cubeShader.SetVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	cubeShader.SetVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
	cubeShader.SetVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

	//cubeShader.SetVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	//cubeShader.SetVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	//cubeShader.SetVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));

	// point light 1
	cubeShader.SetVec3("pointLights[0].position", camera.GetViewMatrix()* glm::vec4(pointLightPositions[0], 1.0f));
	cubeShader.SetVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	cubeShader.SetVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	cubeShader.SetVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	cubeShader.SetFloat("pointLights[0].constant", 1.0f);
	cubeShader.SetFloat("pointLights[0].linear", 0.09);
	cubeShader.SetFloat("pointLights[0].quadratic", 0.032);
	// point light 2
	cubeShader.SetVec3("pointLights[1].position", camera.GetViewMatrix()* glm::vec4(pointLightPositions[1],1.0f));
	cubeShader.SetVec3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	cubeShader.SetVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	cubeShader.SetVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	cubeShader.SetFloat("pointLights[1].constant", 1.0f);
	cubeShader.SetFloat("pointLights[1].linear", 0.09);
	cubeShader.SetFloat("pointLights[1].quadratic", 0.032);
	// point light 3
	cubeShader.SetVec3("pointLights[2].position", camera.GetViewMatrix()* glm::vec4(pointLightPositions[2], 1.0f));
	cubeShader.SetVec3("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	cubeShader.SetVec3("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	cubeShader.SetVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	cubeShader.SetFloat("pointLights[2].constant", 1.0f);
	cubeShader.SetFloat("pointLights[2].linear", 0.09);
	cubeShader.SetFloat("pointLights[2].quadratic", 0.032);
	// point light 4
	cubeShader.SetVec3("pointLights[3].position", camera.GetViewMatrix()* glm::vec4(pointLightPositions[3], 1.0f));
	cubeShader.SetVec3("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	cubeShader.SetVec3("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	cubeShader.SetVec3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	cubeShader.SetFloat("pointLights[3].constant", 1.0f);
	cubeShader.SetFloat("pointLights[3].linear", 0.09);
	cubeShader.SetFloat("pointLights[3].quadratic", 0.032);


	cubeShader.SetVec3("spotLight.position", camera.GetViewMatrix()* glm::vec4(camera.CameraPosition(), 1.0f));
	cubeShader.SetVec3("spotLight.direction", camera.GetViewMatrix()* glm::vec4(camera.CameraDirection(), 0.0f));
	cubeShader.SetVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	cubeShader.SetVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	cubeShader.SetVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	cubeShader.SetFloat("spotLight.constant", 1.0f);
	cubeShader.SetFloat("spotLight.linear", 0.09);
	cubeShader.SetFloat("spotLight.quadratic", 0.032);
	cubeShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	cubeShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));




	////cubeShader.SetVec3("lightColor", glm::vec3(1.0f));
	////cubeShader.SetVec3("lightPos", lightPos);
	//cubeShader.SetVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	//cubeShader.SetVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
	//cubeShader.SetVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	//cubeShader.SetFloat("light.constant", 1.0f);
	//cubeShader.SetFloat("light.linear", 0.09f);
	//cubeShader.SetFloat("light.quadratic", 0.032f);


	//cubeShader.SetVec3("light.position", camera.GetViewMatrix() * glm::vec4(lightPos,1.0f) );
	
	

	//Texture texture2("Images\\awesomeface.png", GL_RGBA,1);
	//program.use();
	//program.SetInt("texture1", texture.Bind_);
	//program.SetInt("texture2", texture2.Bind_);

	//render loop
	glEnable(GL_DEPTH_TEST);

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

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), (float)Screen_W / (float)Screen_H, 0.1f, 100.0f);

		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		lightShader.use();
		//glUniformMatrix4fv(lightShader.GetUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(lightShader.GetUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(lightShader.GetUniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(lightVAO);

		for (unsigned int i = 0; i < 4; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			//float angle = 20.0f * i;
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(lightShader.GetUniform("model"), 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawArrays(GL_TRIANGLES, 0, 36);
		cubeShader.use();
		model = glm::mat4(1);
		glUniformMatrix4fv(lightShader.GetUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(lightShader.GetUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(lightShader.GetUniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
		
		//cubeShader.SetVec3("light.direction", camera.GetViewMatrix() * glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
		//cubeShader.SetVec3("light.position", camera.GetViewMatrix() * glm::vec4(camera.CameraPosition(), 1.0f));
		//cubeShader.SetVec3("light.direction", camera.GetViewMatrix() * glm::vec4(camera.CameraDirection(), 0.0f));
		//cubeShader.SetFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		//cubeShader.SetFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
		cubeShader.SetVec3("spotLight.position", camera.GetViewMatrix() * glm::vec4(camera.CameraPosition(), 1.0f));
		cubeShader.SetVec3("spotLight.direction", camera.GetViewMatrix() * glm::vec4(camera.CameraDirection(), 0.0f));

		cubeShader.SetVec3("dirLight.direction", camera.GetViewMatrix() * glm::vec4(glm::vec3(-0.2f, -1.0f, -0.3f), 0.0f));
		cubeShader.SetVec3("pointLights[0].position", camera.GetViewMatrix() * glm::vec4(pointLightPositions[0], 1.0f));
		cubeShader.SetVec3("pointLights[1].position", camera.GetViewMatrix() * glm::vec4(pointLightPositions[1], 1.0f));
		cubeShader.SetVec3("pointLights[2].position", camera.GetViewMatrix() * glm::vec4(pointLightPositions[2], 1.0f));
		cubeShader.SetVec3("pointLights[3].position", camera.GetViewMatrix() * glm::vec4(pointLightPositions[3], 1.0f));
		
		diffuse.Bind();
		specular.Bind();

		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(cubeShader.GetUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//texture2.Bind();
		//texture.Bind();


		//check event triggers
		glfwPollEvents();
		glBindVertexArray(0);


		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow(&show_demo_window);
		//ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		//ImGui::Text("Hello from another window!");
		//if (ImGui::Button("Close Me"))
		//	show_another_window = false;
		//ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//takes the render buffer and display it on the window
		glfwSwapBuffers(window);

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