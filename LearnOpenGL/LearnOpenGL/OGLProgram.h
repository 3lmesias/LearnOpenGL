#pragma once

#ifndef OGLPROGRAM
#define OGLPROGRAM

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
class OGLProgram
{
	char infoLog[512];
public:

	int success;
	unsigned int shaderProgram;
	OGLProgram() {
		shaderProgram = glCreateProgram();
	}
	void AttachShaders(Shader* fragment, Shader* vertex) {
		glAttachShader(shaderProgram, vertex->ShaderId);
		glAttachShader(shaderProgram, fragment->ShaderId);
	}
	bool Link() {
		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			return false;
		}
		return true;
	}
	char*  GetError() {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		return infoLog;
	}

	void Use() {
		if (success) {
			glUseProgram(shaderProgram);
		}
	}
};
#endif // !OGLPROGRAM