#pragma once
#ifndef PLANE
#define PLANE

#include <vector>
#include "Mesh.h"
#include "PrimArray.h"

class Plane {
public:
    Texture texture;
    Plane(Texture textures);
    Plane();
    Plane(std::string type);
    void Draw(Shader2& shader);
private:
    unsigned int VAO, VBO, EBO;
    std::string type;
    void SetupMesh();
};

#endif // !PLANE
