#include "FragmentShader.h"

void FragmentShader::CretateShader(const char* code) {

    ShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(ShaderId, 1, &code, NULL);
    glCompileShader(ShaderId);

    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(ShaderId, 512, NULL, infoLog);
    }
}
char* FragmentShader::GetError() {
    return infoLog;
}