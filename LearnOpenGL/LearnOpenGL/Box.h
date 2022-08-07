#pragma once
#ifndef BOX
#define BOX

#include <vector>
#include "Mesh.h"
#include "PrimArray.h"

class Box {
public:
    std::vector<Texture> textures;
    Box(std::vector<Texture> textures);
    void Draw(Shader2& shader);
private:
    unsigned int VAO, VBO, EBO;
    void SetupMesh();
};

#endif // !BOX

