#include "skybox.h"

void SkyBox::load(string const& dirPath)
{
    const string TEXTURE_SKYBOX_FRONT  = dirPath + "//front.tga";
    const string TEXTURE_SKYBOX_BACK   = dirPath + "//back.tga";
    const string TEXTURE_SKYBOX_LEFT   = dirPath + "//left.tga";
    const string TEXTURE_SKYBOX_RIGHT  = dirPath + "//right.tga";
    const string TEXTURE_SKYBOX_TOP    = dirPath + "//top.tga";
    const string TEXTURE_SKYBOX_BOTTOM = dirPath + "//bottom.tga";

    int size = 100.f;
    glm::vec3 vSkyBoxVertices[24] =
    {
            // Front face
            //vec3(-1.f, -1.f, 0.f), vec3(-1.f, 1.f, 0.f), vec3(1.f, -1.f, 0.f), vec3(1.f, 1.f, 0.f),
            glm::vec3(size, size, -size), glm::vec3(size, -size, -size), glm::vec3(-size, size, -size), glm::vec3(-size, -size, -size),
            // Back face
            glm::vec3(-size, size, size), glm::vec3(-size, -size, size), glm::vec3(size, size, size), glm::vec3(size, -size, size),
            // Left face
            glm::vec3(-size, size, -size), glm::vec3(-size, -size, -size), glm::vec3(-size, size, size), glm::vec3(-size, -size, size),
            // Right face
            glm::vec3(size, size, size), glm::vec3(size, -size, size), glm::vec3(size, size, -size), glm::vec3(size, -size, -size),
            // Top face
            glm::vec3(size, size, size), glm::vec3(size, size, -size), glm::vec3(-size, size, size), glm::vec3(-size, size, -size),
            // Bottom face
            glm::vec3(size, -size, -size), glm::vec3(-size, -size, -size), glm::vec3(size, -size, size), glm::vec3(-size, -size, size),
    };
    glm::vec2 vSkyBoxTexCoords[4] =
    {
            glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f)
    };

    glm::vec3 vSkyBoxNormals[6] =
    {
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
    };

    vector<BYTE> data;
    for (int i = 0; i < 24; ++i) {
        data.insert(data.end(), (BYTE*)(&vSkyBoxVertices[i]), (BYTE*)(&vSkyBoxVertices[i]) + sizeof(vec3));
        data.insert(data.end(), (BYTE*)(&vSkyBoxNormals[i / 4]), (BYTE*)(&vSkyBoxNormals[i / 4]) + sizeof(vec3));
        data.insert(data.end(), (BYTE*)(&vSkyBoxTexCoords[i % 4]), (BYTE*)(&vSkyBoxTexCoords[i % 4]) + sizeof(vec2));
    }

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_);
    glBufferData(GL_ARRAY_BUFFER, data.size(), &data[0], GL_STATIC_DRAW);

    for (int i = 0; i < 3; ++i) {
        glEnableVertexAttribArray(i);
    }
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3) + sizeof(vec2), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3) + sizeof(vec2), (GLvoid*)(sizeof(vec3)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3) + sizeof(vec2), (GLvoid*)(2 * sizeof(vec3)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    for (int i = 0; i < 6; ++i) {
        textures_[i].init();
    }

    textures_[0].loadTexture(TEXTURE_SKYBOX_FRONT);
    textures_[1].loadTexture(TEXTURE_SKYBOX_BACK);
    textures_[2].loadTexture(TEXTURE_SKYBOX_LEFT);
    textures_[3].loadTexture(TEXTURE_SKYBOX_RIGHT);
    textures_[4].loadTexture(TEXTURE_SKYBOX_TOP);
    textures_[5].loadTexture(TEXTURE_SKYBOX_BOTTOM);

    for (int i = 0; i < 6; ++i) {
        textures_[i].setFiltering(TEXTURE_FILTER_MIN_LINEAR, TEXTURE_FILTER_MAG_LINEAR);
        textures_[i].setSamplerProp(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        textures_[i].setSamplerProp(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    vertexShader_.createShader(GL_VERTEX_SHADER, "shaders//skybox.vert");
    fragmentShader_.createShader(GL_FRAGMENT_SHADER, "shaders//skybox.frag");

    renderProgram_.createProgram();
    renderProgram_.addShader(&vertexShader_);
    renderProgram_.addShader(&fragmentShader_);
    renderProgram_.linkProgram();
}

void SkyBox::render(SceneSettings const& settings)
{
    glDepthMask(0);

    renderProgram_.useProgram();
    renderProgram_.setUniform("mvp", settings.MVP);
    renderProgram_.setUniform("tSampler", 0);

    glBindVertexArray(vao_);
    for (int i = 0; i < 6; i+=1) {
        textures_[i].bindTexture(0);
        glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
    }

    glDepthMask(1);
}
