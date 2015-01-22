#ifndef MODEL_H
#define MODEL_H

#include "common.h"
#include "shaders.h"
#include "texture.h"
#include "tiny_obj_loader.h"


class Model
{
    GLuint buffer_, vao_;
    
    WShader vertexShader_, fragmentShader_;
    WProgram programRender_;
    
    vector<tinyobj::shape_t> shapes_;
    vector<tinyobj::material_t> materials_;
    
    Texture texture_;
    
    mat4 mvp_;

public:
    void load(string const& objFilePath, string const& textureFilePath);
    void setMVP(mat4 const& mvp);
    void render();
};

#endif //MODEL_H