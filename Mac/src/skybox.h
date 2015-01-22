#ifndef SKYBOX_H
#define SKYBOX_H

#include "common.h"
#include "shaders.h"
#include "texture.h"

class SkyBox
{
    GLuint buffer_;
    GLuint vao_;

    WShader vertexShader_;
    WShader fragmentShader_;
    WProgram renderProgram_;

    Texture textures_[6];

    mat4 mvp_;

public:
    void load(string const& dirPath);
    void setMVP(mat4 const& mvp);
    void render();
};

#endif //SKYBOX_H