#version 330

layout (location = 0) in float vRandInit;
layout (location = 1) in vec3  vPositionInit;
layout (location = 2) in vec3  vVelocityInit;
layout (location = 3) in vec3  vColorInit;
layout (location = 4) in float vFullLifeTime;
layout (location = 5) in float vActualLifeTime;
layout (location = 6) in float vMinSize;
layout (location = 7) in float vMaxSize;
layout (location = 8) in float vOpacityInit;

out float gRandInit;
out vec3  gPositionInit;
out vec3  gVelocityInit;
out vec3  gColorInit;
out float gFullLifeTime;
out float gActualLifeTime;
out float gMinSize;
out float gMaxSize;
out float gOpacityInit;

void main()
{
    gRandInit = vRandInit;
    gPositionInit = vPositionInit;
    gVelocityInit = vVelocityInit;
    gColorInit = vColorInit;
    gFullLifeTime = vFullLifeTime;
    gActualLifeTime = vActualLifeTime;
    gMinSize = vMinSize;
    gMaxSize = vMaxSize;
    gOpacityInit = vOpacityInit;
}

