#pragma once
#ifndef SHADER2_H
#define SHADER2_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader2
{
public:
	unsigned int ShaderId;
	bool success;
	char infoLog[512];

	Shader2(const char* vertexPath, const char* fragmentPath);

	void use();

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	int GetUniform(const std::string& name)const;
};

#endif // !SHADER2_H



