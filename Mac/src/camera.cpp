#include "camera.h"

Camera::Camera()
    : position(vec3(0.f, 0.f, 100.f))
      , view(vec3(0.f, 0.f, 0.f))
      , up(vec3(0.f, 1.f, 0.f))
      , FOV(60.f)
      , nearPlaneDist(10.0f)
      , farPlaneDist(1000.f)
{
}

mat4 Camera::viewMatrix()
{
    return lookAt(position, view, up);
}

mat4 Camera::projMatrix(int width, int height)
{
    return perspective(FOV, 1.f * width / height, nearPlaneDist, farPlaneDist);
}

void Camera::moveForward(float dist)
{
    vec3 direction = view - position;
    vec3 delta = normalize(direction) * dist;
    position = position + delta;
    view = view + delta;
}

void Camera::moveBackward(float dist)
{
    moveForward(-dist);
}

void Camera::rotateUp(float angle)
{
    up = rotate(up, angle, vec3(0.f, 0.f, 1.f));
}

void Camera::changeHeadingAngle(float angle)
{
    vec3 directionView = view - position;
    directionView = rotate(directionView, angle, up);
    view = position + directionView;
}

void Camera::changeElevationAngle(float angle)
{
    vec3 directionView = view - position;
    vec3 norm = cross(directionView, up);
    if (norm == vec3(0.f, 0.f, 0.f)) {
        return;
    }
    directionView = rotate(directionView, angle, norm);
    if (cross(directionView, up) == vec3(0.f, 0.f, 0.f)) {
        return;
    }
    view = position + directionView;
}

void Camera::spinHorizontalAroundViewCenter(float angle)
{
    vec3 spinVector = position - vec3(view.x, position.y, view.z);
    spinVector = rotate(spinVector, angle, up);
    position = vec3(view.x, position.y, view.z) + spinVector;
}

void Camera::spinVerticalAroundViewCenter(float angle)
{
    vec3 spinVector = position - vec3(position.x, view.y, view.z);
    spinVector = rotate(spinVector, angle, vec3(1.f, 0.f, 0.f));
    position = vec3(position.x, view.y, view.z) + spinVector;
}









