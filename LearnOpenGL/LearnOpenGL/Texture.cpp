#include "Texture.h"

TextureC::TextureC(const char* imagePath) {
	unsigned char* data;
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(imagePath, &width, &height, &chanels, 0);
	if (data) {

		GLenum format;

		if (chanels == 1)
			format = GL_RED;
		else if (chanels == 3)
			format = GL_RGB;
		else if (chanels == 4)
			format = GL_RGBA;

		glGenTextures(1, &TextureId);
		glBindTexture(GL_TEXTURE_2D, TextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "Failed to load texture: " << imagePath << std::endl;
	}
	stbi_image_free(data);
}

void TextureC::Bind() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureId);
}