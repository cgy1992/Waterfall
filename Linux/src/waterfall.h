#ifndef WATERFALL_H
#define WATERFALL_H

#include "common.h"
#include "camera.h"
#include "skybox.h"
#include "model.h"
#include "particlesystem.h"

class WaterfallScene
{
    GLFWwindow* window_;
    int width_, height_;

    static bool isLeftPressed;
    static bool isRightPressed;

    static double lastX;
    static double lastY;

    chrono::system_clock::time_point lastFrameTP_;

    SkyBox skyBox_;
    Model mountain_;
    ParticleSystem particleSystem_;

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void error_callback(int error, const char* description);
    
    void clearBuffers();

public:
    static Camera camera;

    WaterfallScene();
    ~WaterfallScene();
    
    void load();

    float updateTimer();
    void initParticleSystem();
    void setupParticleSystem();

    void run();
};

#endif //WATERFALL_H
