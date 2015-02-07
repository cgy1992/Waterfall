#ifndef WATERFALL_SETTINGS_H
#define WATERFALL_SETTINGS_H

#include "common.h"

struct SceneSettings
{
    mat4 MVP;
    mat4 M;
    mat4 V;
    mat4 P;

    float width;
    float height;

    vec3 gravity;

    vec3 lightColor;
    float lightPower;
    vec3 lightPosition_WorldSpace;
    vec3 eyePosition_WorldSpace;
    vec3 viewPosition_WorldSpace;

    void refreshMVP()
    {
        MVP = P * V * M;
    }
};

struct ParticleSystemSettings
{
    int particlesCount;

    vec3 emitterPosition_Average;
    vec3 emitterPosition_Vicinity;
    vec3 velocity_Average;
    vec3 velocity_Vicinity;

    float minLifeTime;
    float maxLifeTime;

    float minSize;
    float maxSize;

    vec3 color;
    float opacity;
};

#endif //WATERFALL_SETTINGS_H
