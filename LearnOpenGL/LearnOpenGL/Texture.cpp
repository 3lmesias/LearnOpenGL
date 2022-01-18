#include "Texture.h"

Texture::Texture(const char* imagePath, int bind) {
	Bind_ = bind;
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
	}
	else {
		std::cout << "Failed to load texture: " << imagePath << std::endl;
	}
	stbi_image_free(data);

}

void Texture::Bind() {
	glActiveTexture(GL_TEXTURE0 + Bind_);
	glBindTexture(GL_TEXTURE_2D, TextureId);
}