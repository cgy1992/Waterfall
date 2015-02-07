#ifndef WATERFALL_H
#define WATERFALL_H

#include "common.h"
#include "camera.h"
#include "skybox.h"
#include "model.h"
#include "particlesystem.h"

#include "lightedtexture.h"

class WaterfallScene
{
    int width_, height_;
    chrono::system_clock::time_point lastFrameTP_;

    TwBar* bar_;

    ParticleSystemSettings psSettings_;
    SceneSettings sceneSettings_;

    LightedTexture ltexture_;
    SkyBox skyBox_;
    Model mountain_;
    ParticleSystem particleSystem_;

    static void error_callback(int error, const char* description);

    void setupDefaultSettings();
    void setupAntTweakBarSettings();

    float updateTimer();

    void ATBInit();

    void clearBuffers();
    void drawSkybox();
    void drawMountain();
    void drawParticles();

public:
    static Camera camera;
    static bool isLeftPressed;
    static bool isRightPressed;

    static double lastX;
    static double lastY;

    WaterfallScene();
    ~WaterfallScene();
    
    void regenerateParticles();
    void initialize();
    void drawFrame();
    void run();
};

#endif //WATERFALL_H