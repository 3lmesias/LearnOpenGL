#pragma once
#ifndef BOX
#define BOX

#include <vector>
#include "Mesh.h"
#include "PrimArray.h"

class Box {
public:
    Texture texture;
    Box(Texture textures);
    Box();
    void Draw(Shader2& shader);
private:
    unsigned int VAO, VBO, EBO;
    void SetupMesh();
};

#endif // !BOX

