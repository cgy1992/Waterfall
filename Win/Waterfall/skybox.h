#ifndef SKYBOX_H
#define SKYBOX_H

#include "common.h"
#include "shaders.h"
#include "texture.h"
#include "settings.h"

class SkyBox
{
    GLuint buffer_;
    GLuint vao_;

    WShader vertexShader_;
    WShader fragmentShader_;
    WProgram renderProgram_;

    Texture textures_[6];

public:
    void initialize();
    void load(string const& dirPath);
    void render(SceneSettings const& settings);
};

#endif //SKYBOX_H