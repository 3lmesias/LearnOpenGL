#pragma once
#ifndef FRAGMENTSHADER
#define FRAGMENTSHADER

#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class FragmentShader :
    public Shader
{
public:
    void CretateShader(const char* code);
    char* GetError();
};

#endif // !FRAGMENTSHADER

