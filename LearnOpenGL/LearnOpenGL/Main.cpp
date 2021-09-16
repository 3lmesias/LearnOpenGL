#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

using namespace std;

int Screen_H = 600;
int Screen_W = 800;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

	// tell open gl the size of the window
	glViewport(0, 0, Screen_W, Screen_H);
	//set resize callback
	//callabaks goes after the window is created and before the render loop
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);




	//triangle 
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};


	unsigned int VBO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
	//GL_STATIC_DRAW : the data is set only once and used many times.
	//GL_DYNAMIC_DRAW : the data is changed a lot and used many times.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//creta shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//check compilation errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "Error shader compilation error \n" << infoLog << endl;
	}


	//render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//Configures color used by glClear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);


		//takes the render buffer and display it on the window
		glfwSwapBuffers(window);
		//check event triggers
		glfwPollEvents();
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
}