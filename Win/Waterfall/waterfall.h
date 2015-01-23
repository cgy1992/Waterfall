#ifndef WATERFALL_H
#define WATERFALL_H

#include "common.h"
#include "camera.h"
#include "skybox.h"
#include "model.h"
#include "particlesystem.h"

class WaterfallScene
{
    int width_, height_;

    chrono::system_clock::time_point lastFrameTP_;

    SkyBox skyBox_;
    Model mountain_;
    ParticleSystem particleSystem_;

    static void error_callback(int error, const char* description);
    
    void clearBuffers();

public:
    static Camera camera;
    static bool isLeftPressed;
    static bool isRightPressed;

    static double lastX;
    static double lastY;

    WaterfallScene();
    ~WaterfallScene();
    
    void load();

    float updateTimer();
    void initParticleSystem();
    void setupParticleSystem();

    void drawFrame();
    void run();
};

#endif //WATERFALL_H