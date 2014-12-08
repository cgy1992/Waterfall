#include "particlesystem.h"
#include "common.h"

Particle::Particle()
    : position(0, 0, 0), velocity(0, 0, 0), color(0, 0, 0), fullLifeTime(0), actualLifeTime(0), size(0), opacity(0)
{}

ParticleSystem::ParticleSystem()
    : _isInitialized(false), _curReadBuffer(0), _particlesData(NULL)
{
    _texture.loadTexture("textures//bang_ta.png", false, 8, 8);
    _texture.bindTexture(0);
    _texture.setFiltering(TEXTURE_FILTER_MAG_LINEAR, TEXTURE_FILTER_MIN_LINEAR);
}

ParticleSystem::~ParticleSystem()
{
    if (_particlesData != NULL) {
        delete _particlesData;
    }
}

void ParticleSystem::generateParticles()
{
    if (_maxParticlesCount <= 0) {
        throw std::runtime_error("Try to generate non-positive count of particles...");
    }

    _particlesDataSize = SERIALIZED_PARTICLE_SIZE * _maxParticlesCount;
    _particlesData = new GLfloat[_particlesDataSize];

    srand(time(NULL));
    size_t index = 0;
    int RAND_PRECISION = RAND_MAX;
    for (int p = 0; p < _maxParticlesCount; ++p) {
        GLfloat initRand = 2.0 * (rand() % RAND_PRECISION) / RAND_PRECISION - 1; //from -1 to 1
        cout << initRand << endl;
        vec3 position = _emitterPosition + _emitterRadius * initRand;
        vec3 velocity = _minVelocity + initRand * _velocityRange;
        vec3 color = _initColor * initRand;
        GLfloat fullLifeTime = _minLifeTime + (_maxLifeTime - _minLifeTime) * abs(initRand);
        GLfloat actualLifeTime = 0;  fullLifeTime * abs(initRand);
        GLfloat size = _minSize + (_maxSize - _minSize) * abs(initRand);
        GLfloat opacity = _initOpacity;

        *(_particlesData + index) = initRand; index += 1;
        *(_particlesData + index) = position.x; index += 1;
        *(_particlesData + index) = position.y; index += 1;
        *(_particlesData + index) = position.z; index += 1;
        *(_particlesData + index) = velocity.x; index += 1;
        *(_particlesData + index) = velocity.y; index += 1;
        *(_particlesData + index) = velocity.z; index += 1;
        *(_particlesData + index) = color.r; index += 1;
        *(_particlesData + index) = color.g; index += 1;
        *(_particlesData + index) = color.b; index += 1;
        *(_particlesData + index) = fullLifeTime; index += 1;
        *(_particlesData + index) = actualLifeTime; index += 1;
        *(_particlesData + index) = size; index += 1;
        *(_particlesData + index) = opacity; index += 1;


/*
        memcpy(_particlesData + index, &initRand, sizeof(GLfloat)); index += sizeof(GLfloat);
        memcpy(_particlesData + index, &position, sizeof(vec3)); index += sizeof(vec3);
        memcpy(_particlesData + index, &velocity, sizeof(vec3)); index += sizeof(vec3);
        memcpy(_particlesData + index, &color, sizeof(vec3)); index += sizeof(vec3);
        memcpy(_particlesData + index, &fullLifeTime, sizeof(GLfloat)); index += sizeof(GLfloat);
        memcpy(_particlesData + index, &actualLifeTime, sizeof(GLfloat)); index += sizeof(GLfloat);
        memcpy(_particlesData + index, &size, sizeof(GLfloat)); index += sizeof(GLfloat);
        memcpy(_particlesData + index, &opacity, sizeof(GLfloat)); index += sizeof(GLfloat);*/
    }

    assert(index == _particlesDataSize);
}

void ParticleSystem::initialize()
{
    if (_isInitialized) {
        return;
    }

    generateParticles();

    const char* varyings[PARTICLE_ATTRIBUTES_COUNT] = {
        "initRandOut",
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
        glTransformFeedbackVaryings(_programUpdate.getProgramId(), PARTICLE_ATTRIBUTES_COUNT, varyings, GL_INTERLEAVED_ATTRIBS);
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
        glBufferData(GL_ARRAY_BUFFER, _particlesDataSize * sizeof(GLfloat), _particlesData, GL_DYNAMIC_DRAW);
        
        for (size_t i = 0; i < PARTICLE_ATTRIBUTES_COUNT; ++i) {
            glEnableVertexAttribArray(i);
        }

        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, SERIALIZED_PARTICLE_SIZE * sizeof(GLfloat), (const GLvoid*)0); //initRand
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, SERIALIZED_PARTICLE_SIZE * sizeof(GLfloat), (const GLvoid*)sizeof(GLfloat)); //position
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, SERIALIZED_PARTICLE_SIZE * sizeof(GLfloat), (const GLvoid*)(4 * sizeof(GLfloat))); //velocity
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, SERIALIZED_PARTICLE_SIZE * sizeof(GLfloat), (const GLvoid*)(7 * sizeof(GLfloat))); //color
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, SERIALIZED_PARTICLE_SIZE * sizeof(GLfloat), (const GLvoid*)(10 * sizeof(GLfloat))); //fullLifeTime
        glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, SERIALIZED_PARTICLE_SIZE * sizeof(GLfloat), (const GLvoid*)(11 * sizeof(GLfloat))); //actualLifeTime
        glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, SERIALIZED_PARTICLE_SIZE * sizeof(GLfloat), (const GLvoid*)(12 * sizeof(GLfloat))); //size
        glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, SERIALIZED_PARTICLE_SIZE * sizeof(GLfloat), (const GLvoid*)(13 * sizeof(GLfloat))); //opacity

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

    _programUpdate.useProgram();
    _programUpdate.setUniform("timePassed", timePassed);
    
    _programUpdate.setUniform("emitterPosition", _emitterPosition);
    _programUpdate.setUniform("emitterRadius", _emitterRadius);

    _programUpdate.setUniform("minVelocity", _minVelocity);
    _programUpdate.setUniform("velocityRange", _velocityRange);
    _programUpdate.setUniform("gravity", _gravity);
    
    _programUpdate.setUniform("minLifeTime", _minLifeTime);
    _programUpdate.setUniform("maxLifeTime", _maxLifeTime);
    _programUpdate.setUniform("minSize", _minSize);
    _programUpdate.setUniform("maxSize", _maxSize);

    _programUpdate.setUniform("initColor", _initColor);
    _programUpdate.setUniform("initOpacity", _initOpacity);

    glEnable(GL_RASTERIZER_DISCARD);

    GLuint query;
    glGenQueries(1, &query);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _transformFeedbackBuffer);

    glBindVertexArray(_VAOs[_curReadBuffer]);
    //glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(2);

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _particlesBuffers[1 - _curReadBuffer]);
    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
    glBeginTransformFeedback(GL_POINTS);

    glDrawArrays(GL_POINTS, 0, _maxParticlesCount);

    glEndTransformFeedback();

    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
    
    _curReadBuffer = 1 - _curReadBuffer;

    GLuint primitives;
    glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitives);
    //cout << "Generated primitives: " << primitives << endl;


    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

    //glBindBuffer(GL_ARRAY_BUFFER, _particlesBuffers[1-_curReadBuffer]);
    //GLfloat data[14 * 3];
    //glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data), data);
    //int i = 0;
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
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

    _programRender.setUniform("texRowCount", _texture.rowCount());
    _programRender.setUniform("texColumnCount", _texture.columnCount());
    _programRender.setUniform("mView", _mView);
    _programRender.setUniform("mProj", _mProj);
    _programRender.setUniform("quad1", _quad1);
    _programRender.setUniform("quad2", _quad2);
    _programRender.setUniform("sampler", _texture.textureUnit());

    _texture.bindTexture(0);

    glBindVertexArray(_VAOs[_curReadBuffer]);
    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(2);

    glDrawArrays(GL_POINTS, 0, _maxParticlesCount);

    //glDepthMask(1);
    //glDisable(GL_BLEND);
}

void ParticleSystem::setMaxParticlesCount(int maxParticlesCount)
{
    _maxParticlesCount = maxParticlesCount;
}

void ParticleSystem::setEmitterPosition(vec3 emitterPosition)
{
    _emitterPosition = emitterPosition;
}

void ParticleSystem::setEmitterRadius(vec3 emitterRadius)
{
    _emitterRadius = emitterRadius;
}

void ParticleSystem::setMinVelocity(vec3 minVelocity)
{
    _minVelocity = minVelocity;
}

void ParticleSystem::setVelocityRange(vec3 velocityRange)
{
    _velocityRange = velocityRange;
}

void ParticleSystem::setGravity(vec3 gravity)
{
    _gravity = gravity;
}

void ParticleSystem::setMinLifeTime(float minLifeTime)
{
    _minLifeTime = minLifeTime;
}

void ParticleSystem::setMaxLifeTime(float maxLifeTime)
{
    _maxLifeTime = maxLifeTime;
}

void ParticleSystem::setMinSize(float minSize)
{
    _minSize = minSize;
}

void ParticleSystem::setMaxSize(float maxSize)
{
    _maxSize = maxSize;
}

void ParticleSystem::setInitColor(vec3 color)
{
    _initColor = color;
}

void ParticleSystem::setInitOpacity(float opacity)
{
    _initOpacity = opacity;
}

void ParticleSystem::setMatrices(mat4 mProj, vec3 eye, vec3 viewCenter, vec3 upVector, quat rotation)
{
    _mProj = mProj;
    _mView = lookAt(eye, viewCenter, upVector); // *mat4_cast(rotation);

    vec3 viewDirection = viewCenter - eye;
    _quad1 = cross(viewDirection, upVector);
    _quad2 = cross(_quad1, viewDirection);
    _quad1 = normalize(_quad1);
    _quad2 = normalize(_quad2);
}

