#pragma once
#ifndef VERTEXSHADER
#define VERTEXSHADER

#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VertexShader :
    public Shader
{
public:
    void CretateShader(const char* code);
    char* GetError();
};
#endif // !VERTEXSHADER