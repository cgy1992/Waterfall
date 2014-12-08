#include "particlesystem.h"
#include "common.h"

Particle::Particle()
    : position(0, 0, 0), velocity(0, 0, 0), color(0, 0, 0), fullLifeTime(0), actualLifeTime(0), size(0), opacity(0)
{}

ParticleSystem::ParticleSystem()
    : _isInitialized(false), _maxParticlesCount(0)
{
    _texture.loadTexture("textures//water_drop.png", false);
    _texture.bindTexture(0);
    _texture.setFiltering(TEXTURE_FILTER_MAG_LINEAR, TEXTURE_FILTER_MIN_LINEAR);
}

ParticleSystem::ParticleSystem(int maxParticlesCount)
    : _maxParticlesCount(maxParticlesCount)
{}

void ParticleSystem::generateParticles()
{
    if (_maxParticlesCount <= 0) {
        return;
    }

    size_t serializeParticleSize = 3 * sizeof(vec3) + 4 * sizeof(GLfloat);
    _particlesData.resize(serializeParticleSize * _maxParticlesCount);
}

void ParticleSystem::setMaxParticlesCount(int maxParticlesCount)
{
    _maxParticlesCount = maxParticlesCount;
}

void ParticleSystem::initialize()
{
    if (_isInitialized) {
        return;
    }

    generateParticles();

    const char* varyings[PARTICLE_ATTRIBUTES_COUNT] = {
        "initRandomOut"
        "positionOut",
        "velocityOut",
        "colorOut",
        "fullLifeTimeOut",
        "actualLifeTimeOut",
        "sizeOut",
        "opacityOut"
    };

    _vertShaderUpdate.createShader(GL_VERTEX_SHADER, "shaders//update.vert");
    _geomShaderUpdate.createShader(GL_GEOMETRY_SHADER, "shaders//update.geom");

    _programUpdate.createProgram();
    _programUpdate.addShader(&_vertShaderUpdate);
    _programUpdate.addShader(&_geomShaderUpdate);
    for (size_t i = 0; i < PARTICLE_ATTRIBUTES_COUNT; ++i) {
        glTransformFeedbackVaryings(_programUpdate.getProgramId(), 7, varyings, GL_INTERLEAVED_ATTRIBS);
    }
    _programUpdate.linkProgram();

    _vertShaderRender.createShader(GL_VERTEX_SHADER, "shaders//render.vert");
    _geomShaderRender.createShader(GL_GEOMETRY_SHADER, "shaders//render.geom");
    _fragShaderRender.createShader(GL_FRAGMENT_SHADER, "shaders//render.frag");

    _programRender.createProgram();
    _programRender.addShader(&_vertShaderRender);
    _programRender.addShader(&_geomShaderRender);
    _programRender.addShader(&_fragShaderRender);
    _programRender.linkProgram();

    glGenTransformFeedbacks(1, &_transformFeedbackBuffer);

    glGenBuffers(2, _particlesBuffers);
    glGenVertexArrays(2, _VAOs);

    for (size_t i = 0; i < 2; ++i) {
        glBindVertexArray(_VAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, _particlesBuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, _particlesData.size(), &_particlesData, GL_DYNAMIC_DRAW);
        
        for (size_t i = 0; i < PARTICLE_ATTRIBUTES_COUNT; ++i) {
            glEnableVertexAttribArray(i);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0); //position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)sizeof(vec3)); //velocity
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(2 * sizeof(vec3))); //color
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(3 * sizeof(vec3))); //fullLifeTime
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(3 * sizeof(vec3) + sizeof(GLfloat))); //actualLifeTime
        glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(3 * sizeof(vec3) + 2 * sizeof(GLfloat))); //size
        glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)(3 * sizeof(vec3) + 3 * sizeof(GLfloat))); //opacity

        glBindVertexArray(0);
    }

    _curReadBuffer = 0;
    _isInitialized = true;
}

void ParticleSystem::updateParticles(float timePassed)
{
    if (!_isInitialized) {
        return;
    }

    srand(time(NULL));

    _programUpdate.useProgram();
    _programUpdate.setUniform("timePassed", timePassed);
    _programUpdate.setUniform("randomSeed", vec3(1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX));
    
    _programUpdate.setUniform("emitterPosition", vec3(0.0f, 0.0f, 0.0f));
    _programUpdate.setUniform("emitterRadius", vec3(5.0f, 5.0f, 10.0f));

    _programUpdate.setUniform("minVelocity", vec3(2.0f, 2.0f, 0.0f));
    _programUpdate.setUniform("velocityRange", vec3(3.0f, 3.0f, 3.0f));
    _programUpdate.setUniform("gravity", vec3(0.0f, -2.0f, 0.0f));
    
    _programUpdate.setUniform("minLifeTime", 1.0f);
    _programUpdate.setUniform("maxLifeTime", 10.0f);
    _programUpdate.setUniform("minSize", 2.0f);
    _programUpdate.setUniform("maxSize", 4.0f);

    _programUpdate.setUniform("initColor", vec3(0.0f, 0.0f, 1.0f));
    _programUpdate.setUniform("initOpacity", vec3(0.0f, 0.0f, 1.0f));

    glBindBuffer(GL_ARRAY_BUFFER, _particlesBuffers[_curReadBuffer]);
    float* a = ((float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE) + 1);
    for (int i = 0; i < 52; ++i) {
        float c = *(a + i);
        float d = c;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_RASTERIZER_DISCARD);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _transformFeedbackBuffer);

    glBindVertexArray(_VAOs[_curReadBuffer]);
    glEnableVertexAttribArray(1);

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _particlesBuffers[1 - _curReadBuffer]);
    glBeginTransformFeedback(GL_POINTS);

    glDrawArrays(GL_POINTS, 0, _maxParticlesCount);

    glEndTransformFeedback();

    _curReadBuffer = 1 - _curReadBuffer;
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

    //for debug
    glBindBuffer(GL_ARRAY_BUFFER, _particlesBuffers[_curReadBuffer]);
    float* b = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::renderParticles()
{
    if (!_isInitialized) {
        return;
    }

    _programRender.useProgram();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(0);

    glDisable(GL_RASTERIZER_DISCARD);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 0.5);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _programRender.setUniform("texRowCount", 1);
    _programRender.setUniform("texColumnCount", 1);
    _programRender.setUniform("mView", _mView);
    _programRender.setUniform("mProj", _mProj);
    _programRender.setUniform("quad1", _quad1);
    _programRender.setUniform("quad2", _quad2);
    _programRender.setUniform("sampler", 0);

    _texture.bindTexture(0);

    glBindVertexArray(_VAOs[_curReadBuffer]);
    glDisableVertexAttribArray(1);

    glDrawArrays(GL_POINTS, 0, _maxParticlesCount);

    //glDepthMask(1);
    //glDisable(GL_BLEND);

}

void ParticleSystem::setMatrices(mat4 mProj, vec3 eye, vec3 viewCenter, vec3 upVector)
{
    _mProj = mProj;
    _mView = lookAt(eye, viewCenter, upVector);

    vec3 viewDirection = viewCenter - eye;
    _quad1 = cross(viewDirection, upVector);
    _quad2 = cross(_quad1, viewDirection);
    _quad1 = normalize(_quad1);
    _quad2 = normalize(_quad2);
}

