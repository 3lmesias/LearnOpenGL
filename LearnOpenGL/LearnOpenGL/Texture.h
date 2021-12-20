#pragma once

#ifndef TEXTURE
#define TEXTURE

#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Texture
{
public:
	int width;
	int height;
	int chanels;
	int Bind_;
	
	unsigned int TextureId;

	Texture(const char* imagePath, int format, int bind);
	void Bind();
};
#endif