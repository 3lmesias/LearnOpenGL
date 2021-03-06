#include "VertexShader.h"


void VertexShader::CretateShader(const char* code) {
    ShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(ShaderId, 1, &code, NULL);
    glCompileShader(ShaderId);

    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(ShaderId, 512, NULL, infoLog);
    }
}
char* VertexShader::GetError() {
    return infoLog;
}
