#include "waterfall.h"

double WaterfallScene::lastX = 0;
double WaterfallScene::lastY = 0;
bool WaterfallScene::isLeftPressed = false;
bool WaterfallScene::isRightPressed = false;

Camera WaterfallScene::camera;

WaterfallScene::WaterfallScene()
    : lastFrameTP_(chrono::system_clock::now())
{
}

void WaterfallScene::error_callback(int error, const char *description)
{
    cout << "ERROR: " << description << endl;
}

void WaterfallScene::clearBuffers()
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TW_CALL regenerate_callback(void* data)
{
    WaterfallScene* waterfall = (WaterfallScene*)data;
    waterfall->regenerateParticles();
    cout << "CALLBACK" << endl;
}

void WaterfallScene::regenerateParticles()
{
    cout << psSettings_.particlesCount << endl;
    particleSystem_.initialize(psSettings_);
}

void WaterfallScene::initialize()
{
    ATBInit();

    setupDefaultSettings();

    skyBox_.load("textures//desert");
    mountain_.load("models//bridge.obj", "textures//bridge.jpg");
    particleSystem_.initialize(psSettings_);
}

void WaterfallScene::ATBInit()
{
    TwInit(TW_OPENGL, NULL);

    bar_ = TwNewBar("Parameters");
    TwDefine(" Parameters size='500 500' color='70 100 120' valueswidth=220 iconpos=topleft");

    TwStructMember vec3Members[] = {
        { "x", TW_TYPE_FLOAT, offsetof(vec3, x), "step=0.5" },
        { "y", TW_TYPE_FLOAT, offsetof(vec3, y), "step=0.5" },
        { "z", TW_TYPE_FLOAT, offsetof(vec3, z), "step=0.5" }
    };
    TwType vec3Type;
    vec3Type = TwDefineStruct("vec3Type", vec3Members, 3, sizeof(vec3), NULL, NULL);

    TwAddVarRW(bar_, "Particles_Count", TW_TYPE_INT32, &psSettings_.particlesCount, "step=1");

    TwAddVarRW(bar_, "Emitter_Position", vec3Type, &psSettings_.emitterPosition_Average, NULL);
    TwAddVarRW(bar_, "Emitter_Position_Range", vec3Type, &psSettings_.emitterPosition_Vicinity, NULL);
    TwAddVarRW(bar_, "Velocity", vec3Type, &psSettings_.velocity_Average, NULL);
    TwAddVarRW(bar_, "Velocity_Range", vec3Type, &psSettings_.velocity_Vicinity, NULL);

    TwAddVarRW(bar_, "Min_LifeTime", TW_TYPE_FLOAT, &psSettings_.minLifeTime, "step=0.5");
    TwAddVarRW(bar_, "Max_LifeTime", TW_TYPE_FLOAT, &psSettings_.maxLifeTime, "step=0.5");
    TwAddVarRW(bar_, "Min_Size", TW_TYPE_FLOAT, &psSettings_.minSize, "step=0.5");
    TwAddVarRW(bar_, "Max_Size", TW_TYPE_FLOAT, &psSettings_.maxSize, "step=0.5");

    TwAddVarRW(bar_, "Particle_Color", TW_TYPE_COLOR3F, &psSettings_.color, NULL);
    TwAddVarRW(bar_, "Particle_Opacity", TW_TYPE_FLOAT, &psSettings_.opacity, "min=0 max=1 step=0.01");

    TwAddButton(bar_, "ReGenerate", (TwButtonCallback)regenerate_callback, (void*)this, "label='Regenerate particles'");

    TwAddVarRW(bar_, "Gravity", vec3Type, &sceneSettings_.gravity, NULL);
}

void WaterfallScene::drawFrame()
{
    float const width = (float)glutGet(GLUT_WINDOW_WIDTH);
    float const height = (float)glutGet(GLUT_WINDOW_HEIGHT);

    sceneSettings_.P = WaterfallScene::camera.projMatrix(width, height);
    sceneSettings_.V = WaterfallScene::camera.viewMatrix(); 
    sceneSettings_.width = width;
    sceneSettings_.height = height;

    clearBuffers();

    /* Skybox rendering */
    drawSkybox();

    /* Mountain rendering */
    drawMountain();

    /* Waterfall's particles rendering */
    sceneSettings_.gravity = vec3(0.0, -9.0, 0.0);
    sceneSettings_.lightPosition_WorldSpace = camera.position + vec3(0.f, 10.f, 0.f);
    sceneSettings_.eyePosition_WorldSpace = camera.position;
    sceneSettings_.viewPosition_WorldSpace = camera.view;
    sceneSettings_.lightColor = vec3(1.0f, 1.0f, 1.0f);
    sceneSettings_.lightPower = 10000.0;
    particleSystem_.renderParticles(updateTimer(), sceneSettings_);
}

void WaterfallScene::drawSkybox()
{
    sceneSettings_.M = translate(mat4(), WaterfallScene::camera.position);
    sceneSettings_.refreshMVP();
    skyBox_.render(sceneSettings_);
}

void WaterfallScene::drawMountain()
{
    sceneSettings_.M = translate(mat4(), vec3(0.f, -10.f, 0.f));
    sceneSettings_.refreshMVP();
    mountain_.render(sceneSettings_);
}

void WaterfallScene::drawParticles()
{
    sceneSettings_.M = mat4();
    particleSystem_.renderParticles(updateTimer(), sceneSettings_);
}

void WaterfallScene::setupDefaultSettings()
{
    float _cameraZPosition = 100;
    float _cameraFOV = 60.0f;
     
    psSettings_.particlesCount = 2000;

    psSettings_.emitterPosition_Average = vec3(0.0f, 30.0f, 13.f);
    psSettings_.emitterPosition_Vicinity = vec3(10.0f, 0.0f, 0.0f);
    psSettings_.velocity_Average = vec3(0.0f, -0.0f, 0.0f);
    psSettings_.velocity_Vicinity = vec3(5.0f, 0.0f, 0.0f);

    psSettings_.minLifeTime = 7;
    psSettings_.maxLifeTime = 10;
    psSettings_.minSize = 5;
    psSettings_.maxSize = 5;

    psSettings_.color = vec3(0.0f, 1.f, 1.0f);
    psSettings_.opacity = 0.4f;
}

void WaterfallScene::setupAntTweakBarSettings()
{
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