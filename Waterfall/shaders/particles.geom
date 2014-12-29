#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in float gRandInit[];
in vec3  gPositionInit[];
in vec3  gVelocityInit[];
in vec3  gColorInit[];
in float gFullLifeTime[];
in float gActualLifeTime[];
in float gMinSize[];
in float gMaxSize[];
in float gOpacityInit[];

out vec4  colorFrag;
out vec2  texPrevCoord;
out vec2  texNextCoord;
out float texNextSimilarity;

uniform float timePassed;
uniform vec3  gravity;

uniform int texRowCount;
uniform int texColumnCount;

uniform mat4 mView;
uniform mat4 mProj;

uniform vec3 quad1;
uniform vec3 quad2;

float computeSize(float relativeLifeTime, float minSize, float maxSize)
{
    return minSize + (maxSize - minSize) * relativeLifeTime;
}

float computeOpacity(float relativeLifeTime)
{
    if (relativeLifeTime < 0.3) {
        return 0.4 + 2 * relativeLifeTime;
    }
    if (relativeLifeTime < 0.6) {
        return 1.0;
    }
    if (relativeLifeTime < 1) {
        return 2.5 - 2.5 * relativeLifeTime;
    }
}

vec4 computeColor(float relativeLifeTime, vec3 color, float opacity)
{
    return vec4(color, opacity);
}

/* magic random function obtained by Internet browsing */
vec3 localSeed;
float random01()
{
    uint n = floatBitsToUint(localSeed.y * 214013.0 + localSeed.x * 2531011.0 + localSeed.z * 141251.0);
    n = n * (n * n * 15731u + 789221u);
    n = (n >> 9u) | 0x3F800000u;

    float res =  2.0 - uintBitsToFloat(n);
    localSeed = vec3(localSeed.x + 147158.0 * res, localSeed.y * res  + 415161.0 *res, localSeed.z + 324154.0 * res);
    return res;
}

struct TextureCoords {
    vec2 leftBottom;
    vec2 leftTop;
    vec2 rightBottom;
    vec2 rightTop;
};

TextureCoords getTextureCoords(int texNum)
{
    float texRowNum    = float(texNum / texColumnCount);
    float texColumnNum = float(texNum % texColumnCount);

    TextureCoords texCoords;
    texCoords.leftBottom  = vec2( texColumnNum      / texColumnCount, 1 - (texRowNum + 1) / texRowCount);
    texCoords.leftTop     = vec2( texColumnNum      / texColumnCount, 1 -  texRowNum      / texRowCount);
    texCoords.rightBottom = vec2((texColumnNum + 1) / texColumnCount, 1 - (texRowNum + 1) / texRowCount);
    texCoords.rightTop    = vec2((texColumnNum + 1) / texColumnCount, 1 -  texRowNum      / texRowCount);

    return texCoords;
}

void main()
{
    float randInit = gRandInit[0];
    vec3  positionInit = gPositionInit[0];
    vec3  velocityInit = gVelocityInit[0];
    vec3  colorInit = gColorInit[0];
    float fullLifeTime = gFullLifeTime[0];
    float actualLifeTime = mod(gActualLifeTime[0] + timePassed, fullLifeTime);
    float minSize = gMinSize[0];
    float maxSize = gMaxSize[0];
    float opacityInit = gOpacityInit[0];
    float relativeLifeTime = actualLifeTime / fullLifeTime;

    vec3 position = positionInit + velocityInit * actualLifeTime + gravity * pow(actualLifeTime, 2) / 2;
    float size = computeSize(relativeLifeTime, minSize, maxSize);
    float opacity = computeOpacity(relativeLifeTime);
    
    colorFrag = computeColor(relativeLifeTime, colorInit, opacity);
    mat4 mVP = mProj * mView;
    int texCount = texRowCount * texColumnCount;

    float texNum = relativeLifeTime * texCount;
    int texPrevNum = int(floor(texNum)) % texCount; //textures enumerated from 0 to texCount - 1
    int texNextNum = (texPrevNum + 1) % texCount;

    texNextSimilarity = fract(texNum);

    TextureCoords texPrevCoords = getTextureCoords(texPrevNum);
    TextureCoords texNextCoords = getTextureCoords(texNextNum);

    vec3 posCenter = position;
    vec3 posLeftBottom = posCenter + (-quad1 - quad2) * size;
    texPrevCoord = texPrevCoords.leftBottom;
    texNextCoord = texNextCoords.leftBottom;
    gl_Position = mVP * vec4(posLeftBottom, 1.0);
    EmitVertex();

    vec3 posLeftTop = posCenter + (-quad1 + quad2) * size;
    texPrevCoord = texPrevCoords.leftTop;
    texNextCoord = texNextCoords.leftTop;
    gl_Position = mVP * vec4(posLeftTop, 1.0);
    EmitVertex();

    vec3 posRightBottom = posCenter + (quad1 - quad2) * size;
    texPrevCoord = texPrevCoords.rightBottom;
    texNextCoord = texNextCoords.rightBottom;
    gl_Position = mVP * vec4(posRightBottom, 1.0);
    EmitVertex();

    vec3 posRightTop = posCenter + (quad1 + quad2) * size;
    texPrevCoord = texPrevCoords.rightTop;
    texNextCoord = texNextCoords.rightTop;
    gl_Position = mVP * vec4(posRightTop, 1.0);
    EmitVertex();

    EndPrimitive();

}

