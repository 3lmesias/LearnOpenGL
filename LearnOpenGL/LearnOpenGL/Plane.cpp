#include "Plane.h"

Plane::Plane(Texture textures) {
	this->texture = textures;
	SetupMesh();
}

Plane::Plane(std::string type) {
	this->type = type;
	SetupMesh();
}

Plane::Plane() {
	SetupMesh();
}

void Plane::SetupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	//sets data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
	//GL_STATIC_DRAW : the data is set only once and used many times.
	//GL_DYNAMIC_DRAW : the data is changed a lot and used many times.
	if (type == "xy") {
		glBufferData(GL_ARRAY_BUFFER, sizeof(PrimArray::xyPlaneVertices) / sizeof(PrimArray::xyPlaneVertices[0]) * sizeof(float), &PrimArray::xyPlaneVertices[0], GL_STATIC_DRAW);

	}
	else {
		glBufferData(GL_ARRAY_BUFFER, sizeof(PrimArray::planeVertices) / sizeof(PrimArray::planeVertices[0]) * sizeof(float), &PrimArray::planeVertices[0], GL_STATIC_DRAW);
	}

	//vertex
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	////normal
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	//texture
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Plane::Draw(Shader2& shader) {
	glBindVertexArray(VAO);
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	shader.SetFloat("material.shininess", 128.0f * 0.4f);

	std::string number;

	if (texture.type != "") {
		std::string name = texture.type;
		if (name == "texture_diffuse") {
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular") {
			number = std::to_string(specularNr++);
		}
		shader.SetFloat(("material." + name + number).c_str(), 0);
		glBindTexture(GL_TEXTURE_2D, texture.id);
	}
	glActiveTexture(GL_TEXTURE0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}