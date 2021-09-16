#pragma once
class Shader
{
public :
	unsigned int ShaderId;
private:
	int success;
	char infoLog[512];
public:
	virtual void CretateShader(char* code) = 0;
	virtual char* GetError() = 0;
};

