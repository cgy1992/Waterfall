#include "particlesystem.h"
#include "common.h"
#include "utils.h"

size_t Particle::serializedByteSize()
{
    static const size_t serializedSize =
        sizeof(GLfloat)    // randInit
        + sizeof(vec3)     // positionInit
        + sizeof(vec3)     // velocityInit
        + sizeof(vec3)     // color
        + sizeof(GLfloat)  // fullLifeTime
        + sizeof(GLfloat)  // actualLifeTime
        + sizeof(GLfloat)  // minSize
        + sizeof(GLfloat)  // maxSize
        + sizeof(GLfloat)  // opacity
        ;

    return serializedSize;
}

size_t Particle::serializedFloatSize()
{
    return serializedByteSize() / sizeof(GLfloat);
}

size_t Particle::serialize(GLfloat* buf)
{
    size_t index = 0;

    index += serializeGLfloat(buf + index, randInit);
    index += serializeVec3(buf + index, positionInit);
    index += serializeVec3(buf + index, velocityInit);
    index += serializeVec3(buf + index, color);
    index += serializeGLfloat(buf + index, fullLifeTime);
    index += serializeGLfloat(buf + index, actualLifeTime);
    index += serializeGLfloat(buf + index, minSize);
    index += serializeGLfloat(buf + index, maxSize);
    index += serializeGLfloat(buf + index, opacity);

    assert(index == PARTICLE_SERIALIZED_GLFLOAT_COUNT);
    return index;
}

ParticleSystem::ParticleSystem()
    : _isInitialized(false)
    , _maxParticlesCount(0)
    , _particlesDataSize(0)
    , _particlesData(NULL)
{
}

ParticleSystem::~ParticleSystem()
{
    if (_particlesData != NULL) {
        delete _particlesData;
    }
}

void ParticleSystem::loadTextureAtlas(string const& fileName, size_t rowCount, size_t columnCount)
{
    textureParticle_.init();
    textureParticle_.loadTexture(fileName, false, rowCount, columnCount);
//    textureParticle_.bindTexture(0);
    textureParticle_.setFiltering(TEXTURE_FILTER_MAG_LINEAR, TEXTURE_FILTER_MIN_LINEAR);
}

void ParticleSystem::generateParticles()
{
    if (_maxParticlesCount <= 0) {
        throw std::runtime_error("Try to generate non-positive count of particles...");
    }

    _particlesDataSize = PARTICLE_SERIALIZED_GLFLOAT_COUNT * _maxParticlesCount;
    _particlesData     = new GLfloat[_particlesDataSize];

    size_t offset = 0;
    int RAND_PRECISION = RAND_MAX;
    for (size_t p = 0; p < _maxParticlesCount; ++p) {
        Particle particle;
        particle.randInit = getRandomRange(-1, 1, RAND_PRECISION);
        particle.positionInit = getRandomValueVicinityVec3(emitterPosition, emitterVicinity, RAND_PRECISION);
        particle.velocityInit = getRandomValueVicinityVec3(averageVelocity, velocityVicinity, RAND_PRECISION);
        particle.color = colorInit;// * getRandom01Vec3(RAND_PRECISION);
        particle.fullLifeTime = getRandomRange(minLifeTime, maxLifeTime, RAND_PRECISION);
        particle.actualLifeTime = particle.fullLifeTime * getRandom01(RAND_PRECISION);
        particle.minSize = minSize + (maxSize - minSize) * getRandomRange(0, 0.5, RAND_PRECISION);
        particle.maxSize = maxSize + (maxSize - minSize) * getRandomRange(0, 0.5, RAND_PRECISION);
        particle.opacity = 0;

        offset += particle.serialize(_particlesData + offset);
    }

    assert(offset == _particlesDataSize);
}

void ParticleSystem::initialize(size_t particlesCount)
{
    if (_isInitialized) {
        return;
    }

    _maxParticlesCount = particlesCount;
    generateParticles();

    const char* varyings[PARTICLE_ATTRIBUTES_COUNT] = {
        "vRandInit",
        "vPositionInit",
        "vVelocityInit",
        "vColorInit",
        "vFullLifeTime",
        "vActualLifeTime",
        "vMinSize", "vMaxSize",
        "opacityOut"
    };

    _vertShaderRender.createShader(GL_VERTEX_SHADER, "shaders//particles.vert");
    _geomShaderRender.createShader(GL_GEOMETRY_SHADER, "shaders//particles.geom");
    _fragShaderRender.createShader(GL_FRAGMENT_SHADER, "shaders//particles.frag");

    _programRender.createProgram();
    _programRender.addShader(&_vertShaderRender);
    _programRender.addShader(&_geomShaderRender);
    _programRender.addShader(&_fragShaderRender);
    _programRender.linkProgram();

    glGenBuffers(1, &_particlesBuffer);
    glGenVertexArrays(1, &_particlesVAO);

    glBindVertexArray(_particlesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, _particlesBuffer);
    glBufferData(GL_ARRAY_BUFFER, _particlesDataSize * sizeof(GLfloat), _particlesData, GL_STATIC_DRAW);
        
    for (size_t i = 0; i < PARTICLE_ATTRIBUTES_COUNT; ++i) {
        glEnableVertexAttribArray(i);
    }

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, Particle::serializedByteSize(), (const GLvoid*)(0  * sizeof(GLfloat)));  //randInit
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Particle::serializedByteSize(), (const GLvoid*)(1 * sizeof(GLfloat)));  //positionInit
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, Particle::serializedByteSize(), (const GLvoid*)(4 * sizeof(GLfloat)));  //velocityInit
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, Particle::serializedByteSize(), (const GLvoid*)(7 * sizeof(GLfloat)));  //color
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, Particle::serializedByteSize(), (const GLvoid*)(10 * sizeof(GLfloat)));  //fullLifeTime
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, Particle::serializedByteSize(), (const GLvoid*)(11 * sizeof(GLfloat)));  //actualLifeTime
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, Particle::serializedByteSize(), (const GLvoid*)(12 * sizeof(GLfloat)));  //minSize
    glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, Particle::serializedByteSize(), (const GLvoid*)(13 * sizeof(GLfloat)));  //maxSize
    glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, Particle::serializedByteSize(), (const GLvoid*)(14 * sizeof(GLfloat)));  //opacity

    glBindVertexArray(0);

    _isInitialized = true;

//    textureParticle_.loadTexture("textures//wavemap.png");
//    textureParticle_.setFiltering(TEXTURE_FILTER_MIN_LINEAR, TEXTURE_FILTER_MAG_LINEAR);
    
    textureRefract_.init();
    textureRefract_.loadTexture("textures//wavemap2.png");
    textureRefract_.setFiltering(TEXTURE_FILTER_MIN_LINEAR, TEXTURE_FILTER_MAG_LINEAR);
    
    textureBackground_.init();
}

void ParticleSystem::renderParticles(float timePassed, int width, int height)
{
    if (!_isInitialized) {
        return;
    }

    _programRender.useProgram();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //glDepthMask(0);
    
    textureBackground_.createFromBackground(0, 0, width, height);
    textureBackground_.setFiltering(TEXTURE_FILTER_MIN_LINEAR, TEXTURE_FILTER_MAG_LINEAR);
    textureBackground_.setSamplerProp(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    textureBackground_.setSamplerProp(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    textureParticle_.bindTexture(0);
    textureBackground_.bindTexture(1);
    textureRefract_.bindTexture(2);
    
    _programRender.setUniform("timePassed", timePassed);
    _programRender.setUniform("gravity", gravity);
    _programRender.setUniform("texRowCount", textureParticle_.rowCount());
    _programRender.setUniform("texColumnCount", textureParticle_.columnCount());
    _programRender.setUniform("mView", mView);
    _programRender.setUniform("mProj", mProj);
    _programRender.setUniform("quad1", _quad1);
    _programRender.setUniform("quad2", _quad2);
    
    _programRender.setUniform("tSamplerParticle", 0);
    _programRender.setUniform("tSamplerBackground", 1);
    _programRender.setUniform("tSamplerRefract", 2);
    _programRender.setUniform("iScreenWidth", width);
    _programRender.setUniform("iScreenHeight", height);

    glBindVertexArray(_particlesVAO);
    glDrawArrays(GL_POINTS, 0, _maxParticlesCount);
    glBindVertexArray(0);

    //glDepthMask(1);
    
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void ParticleSystem::setMaxParticlesCount(int maxParticlesCount)
{
    _maxParticlesCount = maxParticlesCount;
}

void ParticleSystem::setMatrices(mat4 mProj, vec3 cameraPosition, vec3 viewCenter, vec3 upVector)
{
    this->mProj = mProj;
    mView = lookAt(cameraPosition, viewCenter, upVector);

    vec3 viewDirection = viewCenter - cameraPosition;
    _quad1 = cross(viewDirection, upVector);
    _quad2 = cross(_quad1, viewDirection);
    _quad1 = vec3(mView * vec4(normalize(_quad1), 0.f));
    _quad2 = vec3(mView * vec4(normalize(_quad2), 0.f));
}

