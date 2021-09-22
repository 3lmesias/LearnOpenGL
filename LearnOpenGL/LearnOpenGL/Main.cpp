#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <fstream>
#include <sstream> 
#include <string> 
#include "Shader.h"
#include "VertexShader.h"
#include "FragmentShader.h"
#include "OGLProgram.h"


using namespace std;

int Screen_H = 600;
int Screen_W = 800;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
" vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"uniform vec4 ourColor;\n"
"void main() {\n"
"	FragColor = ourColor;\n"
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




	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;




	//triangle 
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f
	};


	float vertices2[] = {
	0.5f, 0.5f, 0.0f, // top right
	0.5f, -0.5f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f, // bottom left
	-0.5f, 0.5f, 0.0f // top left
	};

	unsigned int indices[] = { // note that we start from 0!
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
	};




	// bind VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
	//GL_STATIC_DRAW : the data is set only once and used many times.
	//GL_DYNAMIC_DRAW : the data is changed a lot and used many times.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//define attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//creta shader
	//unsigned int vertexShader;
	//vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//glCompileShader(vertexShader);

	////check compilation errors
	//int success;
	//char infoLog[512];
	//glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	//if (!success) {
	//	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	//	cout << "Error shader compilation error \n" << infoLog << endl;
	//}

	std::ifstream fsStream("Fragment Shader\\HelloTriangle._fs");
	std::ifstream vsStream("Vertex Shader\\HelloTriangle._vs");
	std::stringstream ssfs;
	std::stringstream ssvs;
	ssfs << fsStream.rdbuf();
	ssvs << vsStream.rdbuf();
	string auxFragment = ssfs.str();
	string auxVertex = ssvs.str();

	VertexShader vertexShader;
	vertexShader.CretateShader(auxVertex.c_str());

	FragmentShader fragmentShader;
	fragmentShader.CretateShader(auxFragment.c_str());

	if (!vertexShader.IsSuccessful()) {
		cout << "Error v shader compilation error \n" << vertexShader.GetError() << endl;
	}
	if (!fragmentShader.IsSuccessful()) {
		cout << "Error f shader compilation error \n" << fragmentShader.GetError() << endl;
	}
	OGLProgram program;
	program.AttachShaders(&fragmentShader, &vertexShader);

	if (!program.Link()) {
		cout << "Error program link error \n" << program.GetError() << endl;
	}
	program.Use();
	
	//Shaders can be deleted afte linking
	vertexShader.Delete();
	fragmentShader.Delete();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);






	int vertexColorLocation = glGetUniformLocation(program.shaderProgram, "ourColor");
	
	//render loop
	while (!glfwWindowShouldClose(window))
	{
		float timeVAlue = glfwGetTime();
		float greenValue = (sin(timeVAlue) / 2.0f) + 0.5f;

		processInput(window);

		//Configures color used by glClear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		program.Use();
		//first use program before updating uniform
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
}