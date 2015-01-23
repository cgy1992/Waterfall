#include "common.h"
#include "waterfall.h"
#include <iostream>
#include <cstring>
#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>

using std::string;
using std::ifstream;

std::unique_ptr<WaterfallScene> gWaterfallScene;

void displayFunc()
{
    gWaterfallScene->drawFrame();

    glutSwapBuffers();
}

void idleFunc()
{
    glutPostRedisplay();
}

void mousebuttonFunc(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            WaterfallScene::isLeftPressed = true;
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            WaterfallScene::isRightPressed = true;
        }

        WaterfallScene::lastX = x;
        WaterfallScene::lastY = y;
    }
    if (state == GLUT_UP) {
        if (button == GLUT_LEFT_BUTTON) {
            WaterfallScene::isLeftPressed = false;
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            WaterfallScene::isRightPressed = false;
        }
    }
}

void mousemotionFunc(int xpos, int ypos)
{
    if (WaterfallScene::isLeftPressed) {
        WaterfallScene::camera.changeHeadingAngle(-(xpos - WaterfallScene::lastX));
        WaterfallScene::lastX = xpos;
    }
    if (WaterfallScene::isRightPressed) {
        WaterfallScene::camera.changeElevationAngle(-(ypos - WaterfallScene::lastY));
        WaterfallScene::lastY = ypos;
    }
}

void keyboardFunc(unsigned char button, int x, int y)
{
    switch (button) {
    case 'q':
        WaterfallScene::camera.rotateUp(5.f);
        break;
    case 'e':
        WaterfallScene::camera.rotateUp(-5.f);
        break;
    case 's':
        WaterfallScene::camera.moveBackward(3.f);
        break;
    case 'w':
        WaterfallScene::camera.moveForward(3.f);
        break;
    case 'a':
        WaterfallScene::camera.spinHorizontalAroundViewCenter(-3.f);
        break;
    case 'd':
        WaterfallScene::camera.spinHorizontalAroundViewCenter(3.f);
        break;
    case 'z':
        WaterfallScene::camera.spinVerticalAroundViewCenter(3.f);
        break;
    case 'x':
        WaterfallScene::camera.spinVerticalAroundViewCenter(-3.f);
        break;
    case 'r':
        WaterfallScene::camera.position = vec3(0.f, 0.f, 100.f);
        WaterfallScene::camera.view = vec3(0.f, 0.f, 0.f);
        WaterfallScene::camera.up = vec3(0.f, 1.f, 0.f);
        break;
     case 27:
        exit(0);
     default:
         break;
    }
}

void reshapeFunc(int width, int height)
{
    if (width <= 0 || height <= 0) {
        return;
    }
    glViewport(0, 0, width, height);
}

void closeFunc()
{
    gWaterfallScene.reset();
}

int main(int argc, char** argv)
{
    std::cout << "Let get it started" << std::endl;

    srand(time(NULL));

    size_t const default_width = 1024;
    size_t const default_height = 1024;

    glutInit(&argc, argv);
    glutInitWindowSize(default_width, default_height);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 0);
    glutCreateWindow("Waterfall");

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW init failed" << std::endl;
        return 1;
    }

    if (!GLEW_VERSION_3_0) {
        std::cerr << "OpenGL 3.0 not supported" << std::endl;
        return 1;
    }

    glutReshapeFunc(reshapeFunc);
    glutDisplayFunc(displayFunc);
    glutIdleFunc(idleFunc);
    glutCloseFunc(closeFunc);
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mousebuttonFunc);
    glutMotionFunc(mousemotionFunc);
    glutPassiveMotionFunc(mousemotionFunc);

    try {
        gWaterfallScene.reset(new WaterfallScene());
        gWaterfallScene->load();
        glutMainLoop();
    }
    catch (std::exception const& except) {
        std::cout << except.what() << std::endl;
        return 1;
    }

    return 0;
}