#pragma once
#ifndef TEXTUREC
#define TEXTUREC

#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class TextureC
{
public:
	int width;
	int height;
	int chanels;

	unsigned int TextureId;

	TextureC(const char* imagePath);
	void Bind();
};
#endif // !TEXTUREC