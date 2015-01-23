#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

class Camera
{
public:
    vec3 position;
    vec3 view;
    vec3 up;

    float FOV;
    float nearPlaneDist;
    float farPlaneDist;

    Camera();

    mat4 viewMatrix();
    mat4 projMatrix(int width, int height);

    void moveForward(float dist);
    void moveBackward(float dist);
    
    void spinHorizontalAroundViewCenter(float angle);
    void spinVerticalAroundViewCenter(float angle);

    void rotateUp(float angle);

    void changeHeadingAngle(float angle);
    void changeElevationAngle(float angle);
};

#endif //CAMERA_H