#include "waterfall.h"

double WaterfallScene::lastX = 0;
double WaterfallScene::lastY = 0;
bool WaterfallScene::isLeftPressed = false;
bool WaterfallScene::isRightPressed = false;

WaterfallScene::WaterfallScene()
    : lastFrameTP_(chrono::system_clock::now())
{
}

void WaterfallScene::error_callback(int error, const char *description)
{
    cout << "ERROR: " << description << endl;
}


Camera WaterfallScene::camera;

void WaterfallScene::clearBuffers()
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WaterfallScene::load()
{
    skyBox_.load("textures//desert");
    mountain_.load("models//bridge.obj", "textures//bridge.jpg");
    initParticleSystem();
}

void WaterfallScene::initParticleSystem()
{
    setupParticleSystem();
    particleSystem_.loadTextureAtlas("textures//water_sprite.png", 4, 4);
    particleSystem_.initialize(5000);
}

void WaterfallScene::drawFrame()
{
    float const width = (float)glutGet(GLUT_WINDOW_WIDTH);
    float const height = (float)glutGet(GLUT_WINDOW_HEIGHT);

    mat4 mProj = WaterfallScene::camera.projMatrix(width, height);
    mat4 mView = WaterfallScene::camera.viewMatrix();
    mat4 mvp = mProj * mView;

    clearBuffers();

    /* Skybox rendering */
    mat4 trans;
    trans = translate(trans, WaterfallScene::camera.position);
    skyBox_.setMVP(mvp * trans);
    skyBox_.render();

    /* Mountain rendering */
    trans = translate(mat4(), vec3(0.f, -10.f, 0.f));
    mountain_.setMVP(mvp * trans);
    mountain_.render();

    /* Waterfall's particles rendering */
    particleSystem_.setMatrices(mProj, WaterfallScene::camera.position, WaterfallScene::camera.view, WaterfallScene::camera.up);
    particleSystem_.renderParticles(updateTimer(), width, height);
}

void WaterfallScene::setupParticleSystem()
{
    float _cameraZPosition = 100;
    float _cameraFOV = 60.0f;

    vec3 _emitterPosition  = vec3(0.0f, 30.0f, 13.f);
    vec3 _emitterVicinity  = vec3(10.0f, 0.0f, 0.0f);
    vec3 _averageVelocity  = vec3(0.0f, -0.0f, 0.0f);
    vec3 _velocityVicinity = vec3(5.0f, 0.0f, 0.0f);
    vec3 _gravity          = vec3(0.0f, -9.0f, 0.0f);

    float _minLifeTime = 7;
    float _maxLifeTime = 10;
    float _minSize = 1;
    float _maxSize = 5;

    vec3 _particleColor = vec3(0.0f, 1.f, 1.0f);
    float _particleOpacity = 0.4f;

    particleSystem_.emitterPosition = _emitterPosition;
    particleSystem_.emitterVicinity = _emitterVicinity;
    particleSystem_.averageVelocity = _averageVelocity;
    particleSystem_.velocityVicinity = _velocityVicinity;
    particleSystem_.gravity = _gravity;
    particleSystem_.minLifeTime = _minLifeTime;
    particleSystem_.maxLifeTime = _maxLifeTime;
    particleSystem_.minSize = _minSize;
    particleSystem_.maxSize = _maxSize;
    particleSystem_.colorInit = _particleColor;
    particleSystem_.opacityInit = _particleOpacity;
}

float WaterfallScene::updateTimer()
{
    chrono::system_clock::time_point now = chrono::system_clock::now();
    float timePassed = chrono::duration<float>(now - lastFrameTP_).count();

    return timePassed;
}

WaterfallScene::~WaterfallScene()
{
}