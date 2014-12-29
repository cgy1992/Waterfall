
#include "model.h"
#include <fstream>

         using std::fstream;
using std::getline;

typedef vector<int> vint;

static string const VERTEX_HEADER = "v";
static string const TEXTURE_HEADER = "vt";
static string const NORMAL_HEADER = "vn";
static string const FACE_HEADER = "f";

Model::Model()
{}

Model::Model(string const& path)
    : path_(path)
{
    load(path);
}

void Model::load(string const& path)
{
    path_ = path;

    ifstream file(path.c_str());

    if (file.fail()) {
        std::cout << "fail to open file" << std::endl;
        return;
    }

    vvec3 temp_vertices;
    vvec3 temp_normals;
    vvec2 temp_textures;

    vint vertexIndices, textureIndices, normalIndices;

    while (!file.eof()) {
        string line;
        getline(file, line);
        if (line.length() == 0) {
            continue;
        }
        if (strncmp(line.c_str(), TEXTURE_HEADER.c_str(), TEXTURE_HEADER.length()) == 0) {
            stringstream in(line.substr(TEXTURE_HEADER.length()));
            temp_textures.push_back(read_vertex2(in));
        }
        else if (strncmp(line.c_str(), NORMAL_HEADER.c_str(), NORMAL_HEADER.length()) == 0) {
            stringstream in(line.substr(NORMAL_HEADER.length()));
            temp_normals.push_back(read_vertex3(in));
        }
        else if (strncmp(line.c_str(), VERTEX_HEADER.c_str(), VERTEX_HEADER.length()) == 0) {
            stringstream in(line.substr(VERTEX_HEADER.length()));
            temp_vertices.push_back(read_vertex3(in));
        }
        else if (strncmp(line.c_str(), FACE_HEADER.c_str(), FACE_HEADER.length()) == 0) {
            stringstream in(line.substr(FACE_HEADER.length()));

            for (int i = 0; i < 3; ++i) {
                vec3 f = read_vertex3(in);
                vertexIndices.push_back(int(f[0])); textureIndices.push_back(int(f[1])); normalIndices.push_back(int(f[2]));
            }
        }
    }

    for (size_t i = 0; i < vertexIndices.size(); ++i) {
        vertices_.push_back(temp_vertices[--vertexIndices[i]]);
        textures_.push_back(temp_textures[--textureIndices[i]]);
        normals_.push_back(temp_normals[--normalIndices[i]]);
    }
}

size_t Model::vertices_count() const
{
    return vertices_.size();
}

vec2 Model::read_vertex2(stringstream &in)
{
    float u, v;
    in >> u; in.get(); in >> v;

    return vec2(u, v);
}

vec3 Model::read_vertex3(stringstream &in)
{
    float x, y, z;
    in >> x; in.get(); in >> y; in.get(); in >> z;

    return vec3(x, y, z);
}

void Model::initialize()
{
    glGenBuffers(1, &buffer_);
    glGenVertexArrays(1, &vao_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_);

    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * 3 * sizeof(GLfloat), &vertices_[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    glBindVertexArray(0);

    vertShaderRender_.createShader(GL_VERTEX_SHADER, "shaders//rock.vert");
    fragShaderRender_.createShader(GL_FRAGMENT_SHADER, "shaders//rock.frag");

    programRender_.createProgram();
    programRender_.addShader(&vertShaderRender_);
    programRender_.addShader(&fragShaderRender_);
    programRender_.linkProgram();
}

void Model::draw()
{
    programRender_.useProgram();
    programRender_.setUniform("mvp", mvp_);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
    glBindVertexArray(0);
}

