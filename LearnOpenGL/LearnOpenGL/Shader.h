#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Shader
{
public :
	unsigned int ShaderId;
protected:
	int success;
	char infoLog[512];
public:
	virtual void CretateShader(const char* code) = 0;
	virtual char* GetError() = 0;
	virtual bool IsSuccessful();
	virtual void Delete();
};

