#include "lightedtexture.h"

void LightedTexture::load()
{
    vertexShader_.createShader(GL_VERTEX_SHADER, "shaders//lightedtexture.vert");
    fragmentShader_.createShader(GL_FRAGMENT_SHADER, "shaders//lightedtexture.frag");

    renderProgram_.createProgram();
    renderProgram_.addShader(&vertexShader_);
    renderProgram_.addShader(&fragmentShader_);
    renderProgram_.linkProgram();

    vec3 p1 = vec3(-10.f, -10.f, 15.f);
    vec3 p2 = vec3(10.f, -10.f, 15.f);
    vec3 p3 = vec3(-10.f, 10.f, -15.f);
    vec3 p4 = vec3(10.f, 10.f, -15.f);

    vec3 tangent = normalize(p2 - p1);
    vec3 bitangent = normalize(p3 - p1);
    vec3 normal = normalize(cross(tangent, bitangent));

    GLfloat data[56] = {
        p1.x, p1.y, p1.z, 0.f, 0.f, tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z, normal.x, normal.y, normal.z,
        p2.x, p2.y, p2.z, 1.f, 0.f, tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z, normal.x, normal.y, normal.z,
        p3.x, p3.y, p3.z, 0.f, 1.f, tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z, normal.x, normal.y, normal.z,
        p4.x, p4.y, p4.z, 1.f, 1.f, tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z, normal.x, normal.y, normal.z,
    };

    glGenBuffers(1, &vbo_);
    glGenVertexArrays(1, &vao_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * sizeof(data), data, GL_STATIC_DRAW);

    for (int i = 0; i < 5; ++i) {
        glEnableVertexAttribArray(i);
    }
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(vec3) + sizeof(vec2), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(vec3) + sizeof(vec2), (GLvoid*)(sizeof(vec3)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(vec3) + sizeof(vec2), (GLvoid*)(sizeof(vec3) + sizeof(vec2)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(vec3) + sizeof(vec2), (GLvoid*)(2 * sizeof(vec3) + sizeof(vec2)));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(vec3) + sizeof(vec2), (GLvoid*)(3 * sizeof(vec3) + sizeof(vec2)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void LightedTexture::loadDiffuse(string const& filename)
{
    diffuseTexture_.init();
    diffuseTexture_.loadTexture(filename);
    diffuseTexture_.setFiltering(TEXTURE_FILTER_MIN_LINEAR, TEXTURE_FILTER_MAG_LINEAR);
}

void LightedTexture::loadNormal(string const& filename)
{
    normalTexture_.init();
    normalTexture_.loadTexture(filename);
    normalTexture_.setFiltering(TEXTURE_FILTER_MIN_LINEAR, TEXTURE_FILTER_MAG_LINEAR);
}

void LightedTexture::loadSpecular(string const& filename)
{
    specularTexture_.init();
    specularTexture_.loadTexture(filename);
    specularTexture_.setFiltering(TEXTURE_FILTER_MIN_LINEAR, TEXTURE_FILTER_MAG_LINEAR);
}

void LightedTexture::setMVP(mat4 const& mvp)
{
    MVP_ = mvp;
}

void LightedTexture::setM(mat4 const& modelMatrix)
{
    M_ = modelMatrix;
}

void LightedTexture::setV(mat4 const& viewMatrix)
{
    V_ = viewMatrix;
}

void LightedTexture::render()
{
    renderProgram_.useProgram();

    renderProgram_.setUniform("MVP", MVP_);
    renderProgram_.setUniform("M", M_);
    renderProgram_.setUniform("V", V_);
    renderProgram_.setUniform("lightPosition_WorldSpace", vec3(0.f, 0.f, 10.f));

    diffuseTexture_.bindTexture(0);
    normalTexture_.bindTexture(1);
    specularTexture_.bindTexture(2);

    renderProgram_.setUniform("tSamplerDiffuse", 0);
    renderProgram_.setUniform("tSamplerNormal", 1);
    renderProgram_.setUniform("tSamplerSpecular", 2);

    renderProgram_.setUniform("lightColor", vec3(1.f, 1.f, 1.f));
    renderProgram_.setUniform("lightPower", 100.f);

    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

}
