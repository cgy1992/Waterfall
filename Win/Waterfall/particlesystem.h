#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "common.h"
#include "shaders.h"
#include "texture.h"
#include "settings.h"

static const size_t PARTICLE_ATTRIBUTES_COUNT = 9;
static const size_t PARTICLE_SERIALIZED_GLFLOAT_COUNT = 15;

struct Particle
{
    GLfloat randInit;
    vec3 positionInit;
    vec3 velocityInit;
    vec3 color;
    GLfloat fullLifeTime;
    GLfloat actualLifeTime;
    GLfloat maxSize;
    GLfloat minSize;
    GLfloat opacity;

    static size_t serializedByteSize();
    static size_t serializedFloatSize();
    size_t serialize(GLfloat* buf);
};

class ParticleSystem
{
    bool _isInitialized;

    int particlesCount_;
    size_t _particlesDataSize;
    GLfloat* _particlesData;

    WShader _vertShaderRender, _geomShaderRender, _fragShaderRender;
    WProgram _programRender;

    GLuint _particlesBuffer;
    GLuint _particlesVAO;

    TextureAtlas textureParticle_;
    Texture textureNormal_;
    Texture textureSpecular_;
    Texture textureBackground_;
    Texture textureRefract_;

    void generateParticles(ParticleSystemSettings const& settings);
    void texturesInit();

public:
    ParticleSystem();
    ~ParticleSystem();

    void initialize(ParticleSystemSettings const& settings);
    void renderParticles(float timePassed, SceneSettings const& settings);
};

#endif //PARTICLE_SYSTEM_H
