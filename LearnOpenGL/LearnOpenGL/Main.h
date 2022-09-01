#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <fstream>
#include <sstream> 
#include <string> 
#include "Shader.h"
#include "Shader2.h"
#include "VertexShader.h"
#include "FragmentShader.h"
#include "OGLProgram.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Model.h"
#include <filesystem>
#include "PrimArray.h"
#include "box.h"
#include "plane.h"