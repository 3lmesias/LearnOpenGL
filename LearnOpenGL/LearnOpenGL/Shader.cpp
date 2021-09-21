#include "Shader.h"

bool Shader::IsSuccessful() {
	return success;
}

void Shader::Delete() {
	glDeleteShader(ShaderId);
}
