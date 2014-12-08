#version 330

layout (points) in;
layout (points, max_vertices = 40) out;

in float initRand[];
in vec3  position[];
in vec3  velocity[];
in vec3  color[];
in float fullLifeTime[];
in float actualLifeTime[];
in float size[];
in float opacity[];

out float initRandOut;
out vec3  positionOut;
out vec3  velocityOut;
out vec3  colorOut;
out float fullLifeTimeOut;
out float actualLifeTimeOut;
out float sizeOut;
out float opacityOut;

uniform float timePassed;

uniform vec3  emitterPosition;
uniform vec3  emitterRadius;
uniform vec3  gravity;

uniform vec3  minVelocity;
uniform vec3  velocityRange;

uniform float minLifeTime;
uniform float maxLifeTime;

uniform float minSize;
uniform float maxSize;

uniform vec3  initColor;
uniform float initOpacity;

vec3 localSeed;

float computeOpacity(float relativeLifeTime)
{
    if (relativeLifeTime < 0.4) {
        return 0.4 + 1.5 * relativeLifeTime;
    }
    if (relativeLifeTime < 0.7) {
        return 1.0;
    }
    if (relativeLifeTime < 1) {
        return 3.1 - 3.0 * relativeLifeTime;
    }
}

float computeSize(float relativeLifeTime)
{
    return minSize + (maxSize - minSize) * relativeLifeTime;
}

/*magic random function obtained by Internet browsing*/
float random01()
{
    uint n = floatBitsToUint(localSeed.y * 214013.0 + localSeed.x * 2531011.0 + localSeed.z * 141251.0);
    n = n * (n * n * 15731u + 789221u);
    n = (n >> 9u) | 0x3F800000u;

    float res =  2.0 - uintBitsToFloat(n);
    localSeed = vec3(localSeed.x + 147158.0 * res, localSeed.y * res  + 415161.0 *res, localSeed.z + 324154.0 * res);
    return res;
}

float randomFromVec3(vec3 data)
{
    return vec3(data.x * random01(), data.y * random01(), data.z * random01());
}

void main()
{
    initRandOut = initRand[0];
    fullLifeTimeOut = fullLifeTime[0];
    actualLifeTimeOut = actualLifeTime[0] + timePassed;
    if (actualLifeTimeOut < fullLifeTimeOut) {
        positionOut = position[0] + velocity[0] * timePassed + gravity * timePassed * timePassed / 2;
        velocityOut = velocity[0] + gravity * timePassed;
        colorOut = color[0];
        
        float relativeLifeTime = actualLifeTimeOut / fullLifeTimeOut;
        sizeOut = computeSize(relativeLifeTime);
        opacityOut = computeOpacity(relativeLifeTime);
    }
    else {
        positionOut = emitterPosition + emitterRadius * initRandOut;
        velocityOut = minVelocity + velocityRange * initRandOut;
        colorOut = initColor;
        fullLifeTimeOut = minLifeTime + (maxLifeTime - minLifeTime) * abs(initRandOut);
        actualLifeTimeOut = 0;
        sizeOut = minSize + (maxSize - minSize) * abs(initRandOut);
        opacityOut = initOpacity;
    }

    EmitVertex();
    EndPrimitive();
}
