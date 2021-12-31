#include "Shader2.h"

Shader2::Shader2(const char* vertexPath, const char* fragmentPath) {
	success = true;
	std::string fragmentCode;
	std::string vertexCode;
	std::ifstream fShaderFile;
	std::ifstream vShaderFile;

	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();


	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FAILED TO READ SHADER FILE\n" << std::endl;
		success = false;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// compilation
	unsigned int vertex, fragment;
	int compileOk;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &compileOk);
	if (!compileOk) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		success = false;
	}


	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &compileOk);
	if (!compileOk) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		success = false;
	}

	ShaderId = glCreateProgram();
	glAttachShader(ShaderId, vertex);
	glAttachShader(ShaderId, fragment);
	glLinkProgram(ShaderId);
	glGetProgramiv(ShaderId, GL_LINK_STATUS, &compileOk);
	if (!compileOk) {
		glGetProgramInfoLog(ShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		success = false;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader2::use() {
	glUseProgram(ShaderId);
}

void Shader2::SetBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ShaderId, name.c_str()), (int)value);
}
void Shader2::SetInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ShaderId, name.c_str()), (int)value);
}
void Shader2::SetFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ShaderId, name.c_str()), (float)value);
}

void Shader2::SetVec3(const std::string& name, glm::vec3 value) const {
	glUniform3f(glGetUniformLocation(ShaderId, name.c_str()), value.x,value.y, value.z);
}

int Shader2::GetUniform(const std::string& name) const {
	return glGetUniformLocation(ShaderId, name.c_str());
}