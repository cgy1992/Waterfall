#ifndef UTILS_H
#define UTILS_H

#include "common.h"

GLuint createShader(GLenum type, const string& fileName);
GLuint createProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader);

#endif //UTILS_H
