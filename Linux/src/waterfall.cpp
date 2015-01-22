#include "waterfall.h"

double WaterfallScene::lastX = 0;
double WaterfallScene::lastY = 0;
bool WaterfallScene::isLeftPressed = false;
bool WaterfallScene::isRightPressed = false;

WaterfallScene::WaterfallScene()
    : lastFrameTP_(chrono::system_clock::now())
{
    if (!glfwInit()) {
        cout << "ERROR: glfw hasn't been initialized" << endl;
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback(WaterfallScene::error_callback);

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    width_ = 640;
    height_ = 640;
    window_ = glfwCreateWindow(width_, height_, "Waterfall", nullptr, nullptr);
    if (!window_) {
        cout << "ERROR: window_ hasn't been created" << endl;
        exit(EXIT_FAILURE);
    }
    glfwGetFramebufferSize(window_, &width_, &height_);
    glfwMakeContextCurrent(window_);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "ERROR: glew hasn't been initialized" << endl;
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window_, WaterfallScene::key_callback);

    //glfwSetInputMode(window_, GLFW_STICKY_MOUSE_BUTTONS, 1);
    glfwSetMouseButtonCallback(window_, (GLFWmousebuttonfun) WaterfallScene::mouse_button_callback);

    glfwSetCursorPosCallback(window_, (GLFWcursorposfun) WaterfallScene::cursor_pos_callback);

    glfwSetFramebufferSizeCallback(window_, WaterfallScene::framebuffer_size_callback);
    framebuffer_size_callback(window_, width_, height_);
}

void WaterfallScene::error_callback(int error, const char *description)
{
    cout << "ERROR: " << description << endl;
}

void WaterfallScene::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    //int state = glfwGetMouseButton(window, button);
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            isLeftPressed = true;
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            isRightPressed = true;
        }

        glfwGetCursorPos(window, &lastX, &lastY);
    }
    if (action == GLFW_RELEASE) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            isLeftPressed = false;
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            isRightPressed = false;
        }
    }
}

void WaterfallScene::cursor_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (isLeftPressed) {
        camera.changeHeadingAngle(-(xpos - lastX));
        lastX = xpos;
    }
    if (isRightPressed) {
        camera.changeElevationAngle(-(ypos - lastY));
        lastY = ypos;
    }
}

void WaterfallScene::key_callback(GLFWwindow* window_, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, GL_TRUE);
    }

    if (key == GLFW_KEY_Q) {
        WaterfallScene::camera.rotateUp(5.f);
    }
    if (key == GLFW_KEY_E) {
        WaterfallScene::camera.rotateUp(-5.f);
    }
    if (key == GLFW_KEY_S) {
        WaterfallScene::camera.moveBackward(3.f);
    }
    if (key == GLFW_KEY_W) {
        WaterfallScene::camera.moveForward(3.f);
    }
    if (key == GLFW_KEY_A) {
        WaterfallScene::camera.spinHorizontalAroundViewCenter(-3.f);
    }
    if (key == GLFW_KEY_D) {
        WaterfallScene::camera.spinHorizontalAroundViewCenter(3.f);
    }
    if (key == GLFW_KEY_Z) {
        WaterfallScene::camera.spinVerticalAroundViewCenter(3.f);
    }
    if (key == GLFW_KEY_X) {
        WaterfallScene::camera.spinVerticalAroundViewCenter(-3.f);
    }
    if (key == GLFW_KEY_R) {
        camera.position = vec3(0.f, 0.f, 100.f);
        camera.view = vec3(0.f, 0.f, 0.f);
        camera.up = vec3(0.f, 1.f, 0.f);
    }
}

void WaterfallScene::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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
    cout << "11" << endl;
    skyBox_.load("textures//desert");
    cout << "22" << endl;
    mountain_.load("models//bridge.obj", "textures//bridge.jpg");
    cout << "33" << endl;
}

void WaterfallScene::run()
{
    cout << "1" << endl;
    initParticleSystem();

    while(!glfwWindowShouldClose(window_)) {
        int width, height;
        glfwGetFramebufferSize(window_, &width, &height);
        mat4 mProj = WaterfallScene::camera.projMatrix(width, height);
        mat4 mView = WaterfallScene::camera.viewMatrix();
        mat4 mvp = mProj * mView;
        
        clearBuffers();
    cout << "2" << endl;
        
        /* Skybox rendering */
        mat4 trans;
        trans = translate(trans, WaterfallScene::camera.position);
        skyBox_.setMVP(mvp * trans);
        skyBox_.render();
            cout << "3" << endl;
        /* Mountain rendering */
        trans = translate(mat4(), vec3(0.f, -10.f, 0.f));
        mountain_.setMVP(mvp * trans);
        mountain_.render();
            cout << "4" << endl;
        /* Waterfall's particles rendering */
        particleSystem_.setMatrices(mProj, WaterfallScene::camera.position, WaterfallScene::camera.view, WaterfallScene::camera.up);
        particleSystem_.renderParticles(updateTimer(), width, height);
    cout << "5" << endl;

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void WaterfallScene::initParticleSystem()
{
    setupParticleSystem();
    particleSystem_.loadTextureAtlas("textures//water2.jpg", 1, 1);
    particleSystem_.initialize(50000);
}

void WaterfallScene::setupParticleSystem()
{
    float _cameraZPosition = 100;
    float _cameraFOV = 60.0f;

    vec3 _emitterPosition  = vec3(0.0f, 30.0f, 13.f);
    vec3 _emitterVicinity  = vec3(25.0f, 0.0f, 0.0f);
    vec3 _averageVelocity  = vec3(.0f, -0.0f, 0.0f);
    vec3 _velocityVicinity = vec3(.0f, 0.0f, 0.0f);
    vec3 _gravity          = vec3(0.0f, -1.0f, 0.0f);

    float _minLifeTime = 7;
    float _maxLifeTime = 10;
    float _minSize = 0.2;
    float _maxSize = 0.3;

    vec3 _particleColor = vec3(0.0f, 1.f, 0.0f);
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
    glfwDestroyWindow(window_);
    glfwTerminate();
}
