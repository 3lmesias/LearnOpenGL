#pragma once
#ifndef SHADER2_H
#define SHADER2_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	void SetVec3(const std::string& name, glm::vec3 value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;

	int GetUniform(const std::string& name)const;
};

#endif // !SHADER2_H