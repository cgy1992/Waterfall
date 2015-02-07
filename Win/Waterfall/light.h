#ifndef LIGHT_H
#define LIGHT_H

#include "common.h"

struct Light
{
    vec3 lightPosition_WorldSpace;
    vec3 lightColor;
    float lightPower;
};

#endif //LIGHT_H
