#ifndef MODEL_H
#define MODEL_H

#include "common.h"
#include <sstream>
#include "shaders.h"

using std::stringstream;

typedef vector<vec2> vvec2;
typedef vector<vec3> vvec3;
typedef vector<vec4> vvec4;
typedef vector<mat3> vmat3;

struct Model
{
    GLuint buffer_;
    GLuint vao_;
    mat4 mvp_;

    WShader vertShaderRender_, fragShaderRender_;
    WProgram programRender_;

    string path_;

    vvec3 vertices_;
    vvec2 textures_;
    vvec3 normals_;
    vvec3 faces_;

    vec2 read_vertex2(stringstream & in);
    vec3 read_vertex3(stringstream & in);

    //public:
    Model();
    Model(string const& path);

    size_t vertices_count() const;

    void load(string const& path);
    void initialize();
    void draw();
};

#endif //MODEL_H