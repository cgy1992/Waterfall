#include "utils.h"

GLuint createShader(GLenum type, const string& fileName)
{
    ifstream fin(fileName.c_str(), std::ios::binary);
    string fileStr((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

    GLuint const shader = glCreateShader(type);
    char const* source = fileStr.c_str();
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        int infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {
            string buffer;
            buffer.resize(infoLogLength);
            glGetShaderInfoLog(shader, infoLogLength, NULL, &buffer[0]);
            throw std::runtime_error(buffer);
        }
    }

    return shader;
}

GLuint createProgram(GLuint vs, GLuint gs, GLuint fs)
{
    GLuint const program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, gs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        int infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {
            string buffer;
            buffer.resize(infoLogLength);
            glGetProgramInfoLog(program, infoLogLength, NULL, &buffer[0]);
            throw std::runtime_error(buffer);
        }
    }

    return program;

}