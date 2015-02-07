#include "particlesystem.h"
#include "common.h"
#include "utils.h"

static const int PARTICLE_SYSTEM_TEXTUREATLAS_PARTICLE_ROW_COUNT = 4;
static const int PARTICLE_SYSTEM_TEXTUREATLAS_PARTICLE_COLUMN_COUNT = 4;
static const string PARTICLE_SYSTEM_TEXTUREATLAS_PARTICLE_FILENAME = "textures//water_sprite.png";
static const string PARTICLE_SYSTEM_TEXTURE_NORMAL_FILENAME = "textures//wavemap2.png";
static const string PARTICLE_SYSTEM_TEXTURE_SPECULAR_FILENAME = "textures//specular.jpg";
static const string PARTICLE_SYSTEM_TEXTURE_REFRACT_FILENAME = "textures//wavemap2.png";

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

void ParticleSystem::generateParticles(ParticleSystemSettings const& settings)
{
    particlesCount_ = settings.particlesCount;
    _particlesDataSize = PARTICLE_SERIALIZED_GLFLOAT_COUNT * settings.particlesCount;
    _particlesData = new GLfloat[_particlesDataSize];

    size_t offset = 0;
    int RAND_PRECISION = RAND_MAX;
    for (size_t p = 0; p < particlesCount_; ++p) {
        Particle particle;
        particle.randInit = getRandomRange(-1, 1, RAND_PRECISION);
        particle.positionInit = getRandomValueVicinityVec3(settings.emitterPosition_Average, settings.emitterPosition_Vicinity, RAND_PRECISION);
        particle.velocityInit = getRandomValueVicinityVec3(settings.velocity_Average, settings.velocity_Vicinity, RAND_PRECISION);
        particle.color = settings.color;
        particle.fullLifeTime = getRandomRange(settings.minLifeTime, settings.maxLifeTime, RAND_PRECISION);
        particle.actualLifeTime = particle.fullLifeTime * getRandom01(RAND_PRECISION);
        particle.minSize = settings.minSize + (settings.maxSize - settings.minSize) * getRandomRange(0, 0.5, RAND_PRECISION);
        particle.maxSize = settings.maxSize + (settings.maxSize - settings.minSize) * getRandomRange(0, 0.5, RAND_PRECISION);
        particle.opacity = 0;

        offset += particle.serialize(_particlesData + offset);
    }

    assert(offset == _particlesDataSize);
}

void ParticleSystem::initialize(ParticleSystemSettings const& settings)
{
    if (_isInitialized) {
        glDeleteBuffers(1, &_particlesBuffer);
        glDeleteVertexArrays(1, &_particlesVAO);
        delete _particlesData;
    }

    generateParticles(settings);

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

    texturesInit();

    _isInitialized = true;
}

void ParticleSystem::texturesInit()
{
    textureParticle_.init();
    textureParticle_.loadTexture(PARTICLE_SYSTEM_TEXTUREATLAS_PARTICLE_FILENAME, PARTICLE_SYSTEM_TEXTUREATLAS_PARTICLE_ROW_COUNT, PARTICLE_SYSTEM_TEXTUREATLAS_PARTICLE_COLUMN_COUNT);
    textureParticle_.setFiltering(TEXTURE_FILTER_MAG_LINEAR, TEXTURE_FILTER_MIN_LINEAR);

    textureNormal_.init();
    textureNormal_.loadTexture(PARTICLE_SYSTEM_TEXTURE_NORMAL_FILENAME);
    textureNormal_.setFiltering(TEXTURE_FILTER_MIN_LINEAR, TEXTURE_FILTER_MAG_LINEAR);

    textureSpecular_.init();
    textureSpecular_.loadTexture(PARTICLE_SYSTEM_TEXTURE_SPECULAR_FILENAME);
    textureSpecular_.setFiltering(TEXTURE_FILTER_MIN_LINEAR, TEXTURE_FILTER_MAG_LINEAR);

    textureRefract_.init();
    textureRefract_.loadTexture(PARTICLE_SYSTEM_TEXTURE_REFRACT_FILENAME);
    textureRefract_.setFiltering(TEXTURE_FILTER_MIN_LINEAR, TEXTURE_FILTER_MAG_LINEAR);

    textureBackground_.init();
}

void ParticleSystem::renderParticles(float timePassed, SceneSettings const& settings)
{
    _programRender.useProgram();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //glDepthMask(0);
    
    cout << "Width: " << settings.width << " height: " << settings.height << endl;

    textureBackground_.createFromBackground(0, 0, settings.width, settings.height);
    textureBackground_.setFiltering(TEXTURE_FILTER_MIN_LINEAR, TEXTURE_FILTER_MAG_LINEAR);
    textureBackground_.setSamplerProp(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    textureBackground_.setSamplerProp(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    textureParticle_.bindTexture(0);
    textureNormal_.bindTexture(1);
    textureSpecular_.bindTexture(2);
    textureBackground_.bindTexture(3);
    textureRefract_.bindTexture(4);
    
    _programRender.setUniform("timePassed", timePassed);
    _programRender.setUniform("gravity", settings.gravity);

    _programRender.setUniform("texRowCount", textureParticle_.rowCount());
    _programRender.setUniform("texColumnCount", textureParticle_.columnCount());

    _programRender.setUniform("MVP", settings.MVP);
    _programRender.setUniform("M", settings.M);
    _programRender.setUniform("V", settings.V);
    _programRender.setUniform("P", settings.P);

    _programRender.setUniform("lightPosition_WorldSpace", settings.lightPosition_WorldSpace);
    _programRender.setUniform("eyePosition_WorldSpace", settings.eyePosition_WorldSpace);
    _programRender.setUniform("viewPosition_WorldSpace", settings.viewPosition_WorldSpace);
    
    _programRender.setUniform("tSamplerParticle", 0);
    _programRender.setUniform("tSamplerNormal", 1);
    _programRender.setUniform("tSamplerSpecular", 2);
    _programRender.setUniform("tSamplerBackground", 3);
    _programRender.setUniform("tSamplerRefract", 4);

    _programRender.setUniform("lightColor", settings.lightColor);
    _programRender.setUniform("lightPower", settings.lightPower);

    _programRender.setUniform("iScreenWidth", settings.width);
    _programRender.setUniform("iScreenHeight", settings.height);

    glBindVertexArray(_particlesVAO);
    glDrawArrays(GL_POINTS, 0, particlesCount_);
    glBindVertexArray(0);

    //glDepthMask(1);
    
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

