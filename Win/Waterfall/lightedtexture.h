#ifndef LIGHTED_TEXTURE
#define LIGHTED_TEXTURE

#include "common.h"
#include "shaders.h"
#include "texture.h"

class LightedTexture
{
    GLuint vbo_;
    GLuint vao_;

    WShader vertexShader_, fragmentShader_;
    WProgram renderProgram_;

    Texture diffuseTexture_;
    Texture normalTexture_;
    Texture specularTexture_;

    mat4 MVP_;
    mat4 M_;
    mat4 V_;

public:
    void load();
    void loadDiffuse(string const& filename);
    void loadNormal(string const& filename);
    void loadSpecular(string const& filename);
    void setMVP(mat4 const& mvp);
    void setM(mat4 const& modelMatrix);
    void setV(mat4 const& viewMatrix);
    void render();
};

#endif //LIGHTED_TEXTURE
