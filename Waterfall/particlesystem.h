#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "common.h"
#include "shaders.h"
#include "texture.h"

#define PARTICLE_ATTRIBUTES_COUNT 7

struct Particle
{
    Particle();

    float initRandom;
    vec3 position;
    vec3 velocity;
    vec3 color;
    GLfloat fullLifeTime;
    GLfloat actualLifeTime;
    GLfloat size;
    GLfloat opacity;
};

class ParticleSystem
{
    bool _isInitialized;
    int _curReadBuffer;

    vector<Particle> _particles;
    vector<char> _particlesData;
    int _maxParticlesCount;

    vec3 _emitterPosition;
    vec3 _emitterRadius;
    vec3 _minVelocity;
    vec3 _velocityRange;
    vec3 _gravity;
    float _minLifeTime;
    float _maxLifeTime;
    float _minSize;
    float _maxSize;
    vec3 _initColor;
    float _initOpacity;

    WShader _vertShaderUpdate, _geomShaderUpdate, _vertShaderRender, _geomShaderRender, _fragShaderRender;
    WProgram _programUpdate, _programRender;

    GLuint _transformFeedbackBuffer;
    GLuint _particlesBuffers[2];
    GLuint _VAOs[2];

    mat4 _mView, _mProj;
    vec3 _quad1, _quad2;
    
    Texture _texture;
    TextureAtlas _textureAtlas;

public:
    ParticleSystem();
    ParticleSystem(int maxParticleSystem);

    void initialize();
    void generateParticles();
    
    void setMaxParticlesCount(int maxParticlesCount);
    void setEmitterPosition(vec3 emitterPosition);
    void setEmitterRadius(vec3 emitterRadius);
    void setMinVelocity(vec3 minVelocity);
    void setVelocityRange(vec3 velocityRange);
    void setGravity(vec3 gravity);
    void setMinLifeTime(float minLifeTime);
    void setMaxLifeTime(float maxLifeTime);
    void setMinSize(float minSize);
    void setMaxSize(float maxSize);
    void setInitColor(vec3 color);
    void setInitOpacity(float opacity);
    void setMatrices(mat4 mProj, vec3 camera, vec3 view, vec3 upVector);

    void updateParticles(float timePassed);
    void renderParticles();
};

#endif //PARTICLE_SYSTEM_H
