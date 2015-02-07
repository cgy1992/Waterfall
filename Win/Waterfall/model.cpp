#include "model.h"
#include "utils.h"


void Model::render(SceneSettings const& settings)
{
    texture_.bindTexture(0);
    
    programRender_.useProgram();
    programRender_.setUniform("mvp", settings.MVP);
    programRender_.setUniform("tSampler", 0);
    
    glEnable(GL_DEPTH_TEST);
    
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, shapes_[0].mesh.positions.size());
    glBindVertexArray(0);
    
    glDisable(GL_DEPTH_TEST);
}

void Model::load(string const& objFilePath, string const& textureFilePath)
{
    string err = tinyobj::LoadObj(shapes_, materials_, objFilePath.c_str());
    if (!err.empty()) {
        throw std::runtime_error("Failed to loading model");
    }
    
    vector<GLfloat> tinydata;
    for (int i = 0; i < shapes_[0].mesh.indices.size(); ++i) {
        int ind = shapes_[0].mesh.indices[i];
        tinydata.push_back(shapes_[0].mesh.positions[3*ind + 0]);
        tinydata.push_back(shapes_[0].mesh.positions[3*ind + 1]);
        tinydata.push_back(shapes_[0].mesh.positions[3*ind + 2]);
        
        tinydata.push_back(shapes_[0].mesh.normals[3*ind + 0]);
        tinydata.push_back(shapes_[0].mesh.normals[3*ind + 1]);
        tinydata.push_back(shapes_[0].mesh.normals[3*ind + 2]);
        
        tinydata.push_back(shapes_[0].mesh.texcoords[2*ind + 0]);
        tinydata.push_back(shapes_[0].mesh.texcoords[2*ind + 1]);
    }
    
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    
    glGenBuffers(1, &buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_);
    glBufferData(GL_ARRAY_BUFFER, tinydata.size() * sizeof(GLfloat), &tinydata[0], GL_STATIC_DRAW);
    
    for (int i = 0; i < 3; ++i) {
        glEnableVertexAttribArray(i);
    }
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3) + sizeof(vec2), 0); //position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3) + sizeof(vec2), (GLvoid*)(sizeof(vec3))); //normal
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3) + sizeof(vec2), (GLvoid*)(2 * sizeof(vec3))); //texture
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    vertexShader_.createShader(GL_VERTEX_SHADER, "shaders//model.vert");
    fragmentShader_.createShader(GL_FRAGMENT_SHADER, "shaders//model.frag");
    programRender_.createProgram();
    programRender_.addShader(&vertexShader_);
    programRender_.addShader(&fragmentShader_);
    programRender_.linkProgram();
    
    texture_.init();
    texture_.loadTexture(textureFilePath.c_str());
    texture_.setFiltering(TEXTURE_FILTER_MIN_LINEAR, TEXTURE_FILTER_MAG_LINEAR);
}


