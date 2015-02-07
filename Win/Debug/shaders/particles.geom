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

out vec4 colorFrag;
out vec2 texPrevCoord;
out vec2 texNextCoord;
out vec2 texCoord;
out float texNextSimilarity;
out vec3 lightDirection_TangentSpace;
out vec3 eyeDirection_TangentSpace;
out vec3 position_WorldSpace;

uniform float timePassed;
uniform vec3  gravity;

uniform int texRowCount;
uniform int texColumnCount;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform vec3 lightPosition_WorldSpace;
uniform vec3 eyePosition_WorldSpace;
uniform vec3 viewPosition_WorldSpace;

uniform float curvature;

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

void emitParticle(vec3 positionCorner_CameraSpace, vec3 positionCenter_CameraSpace, vec3 lightDirection_CameraSpace, vec3 eyeDirection_CameraSpace)
{
    vec3 tangent_CameraSpace = vec3(1.0, 0.0, 0.0);
    vec3 bitangent_CameraSpace = vec3(0.0, 1.0, 0.0);
    vec3 normal_CameraSpace = mix(normalize(eyeDirection_CameraSpace), normalize(positionCorner_CameraSpace - positionCenter_CameraSpace), 0);
    mat3 TBN = transpose(mat3(tangent_CameraSpace, bitangent_CameraSpace, normal_CameraSpace));

    lightDirection_TangentSpace = TBN * lightDirection_CameraSpace;
    eyeDirection_TangentSpace = TBN * eyeDirection_CameraSpace;

    gl_Position = P * vec4(positionCorner_CameraSpace, 1.0);

    EmitVertex();
}

void main()
{
    mat4 MV = V * M;
    mat3 MV3x3 = mat3(MV);

    vec3 positionInit_ModelSpace = gPositionInit[0].xyz;
    vec3 velocityInit_ModelSpace = gVelocityInit[0].xyz;
    
    vec3 positionInit_CameraSpace = (MV * vec4(positionInit_ModelSpace, 1.0)).xyz;
    vec3 velocityInit_CameraSpace = MV3x3 * velocityInit_ModelSpace;
    vec3 gravity_CameraSpace = gravity;

    float fullLifeTime = gFullLifeTime[0];
    float actualLifeTime = mod(gActualLifeTime[0] + timePassed, fullLifeTime);
    float relativeLifeTime = actualLifeTime / fullLifeTime;

    vec3 positionCenter_CameraSpace = positionInit_CameraSpace + velocityInit_CameraSpace * actualLifeTime + gravity_CameraSpace * pow(actualLifeTime, 2) / 2;

    float minSize = gMinSize[0];
    float maxSize = gMaxSize[0];
    float opacityInit = gOpacityInit[0];
    vec3 colorInit = gColorInit[0];

    float size = computeSize(relativeLifeTime, minSize, maxSize);
    float opacity = computeOpacity(relativeLifeTime);    
    colorFrag = computeColor(relativeLifeTime, colorInit, opacity);

    int texCount = texRowCount * texColumnCount;
    float texNum = relativeLifeTime * texCount;
    int texPrevNum = int(floor(texNum)) % texCount; //textures enumerated from 0 to texCount - 1
    int texNextNum = (texPrevNum + 1) % texCount;

    texNextSimilarity = fract(texNum);

    TextureCoords texPrevCoords = getTextureCoords(texPrevNum);
    TextureCoords texNextCoords = getTextureCoords(texNextNum);

    vec3 lightPosition_CameraSpace = (V * vec4(lightPosition_WorldSpace, 1.0)).xyz;
    vec3 eyePosition_CameraSpace = vec3(0.f, 0.f, 0.f);

    vec3 lightDirection_CameraSpace = lightPosition_CameraSpace - positionCenter_CameraSpace;
    vec3 eyeDirection_CameraSpace = eyePosition_CameraSpace - positionCenter_CameraSpace;

    vec3 quad1_CameraSpace = vec3(1.0, 0.0, 0.0);
    vec3 quad2_CameraSpace = vec3(0.0, 1.0, 0.0);

    vec3 positionLeftBottom_CameraSpace = positionCenter_CameraSpace + (-quad1_CameraSpace - quad2_CameraSpace) * size;
    texPrevCoord = texPrevCoords.leftBottom;
    texNextCoord = texNextCoords.leftBottom;
    texCoord = vec2(0.0, 0.0);
    emitParticle(positionLeftBottom_CameraSpace, positionCenter_CameraSpace, lightDirection_CameraSpace, eyeDirection_CameraSpace);

    vec3 positionLeftTop_CameraSpace = positionCenter_CameraSpace + (-quad1_CameraSpace + quad2_CameraSpace) * size;
    texPrevCoord = texPrevCoords.leftTop;
    texNextCoord = texNextCoords.leftTop;
    texCoord = vec2(0.0, 1.0);
    emitParticle(positionLeftTop_CameraSpace, positionCenter_CameraSpace, lightDirection_CameraSpace, eyeDirection_CameraSpace);

    vec3 positionRightBottom_CameraSpace = positionCenter_CameraSpace + (quad1_CameraSpace - quad2_CameraSpace) * size;
    texPrevCoord = texPrevCoords.rightBottom;
    texNextCoord = texNextCoords.rightBottom;
    texCoord = vec2(1.0, 0.0);
    emitParticle(positionRightBottom_CameraSpace, positionCenter_CameraSpace, lightDirection_CameraSpace, eyeDirection_CameraSpace);

    vec3 positionRightTop_CameraSpace = positionCenter_CameraSpace + (quad1_CameraSpace + quad2_CameraSpace) * size;
    texPrevCoord = texPrevCoords.rightTop;
    texNextCoord = texNextCoords.rightTop;
    texCoord = vec2(1.0, 1.0);
    emitParticle(positionRightTop_CameraSpace, positionCenter_CameraSpace, lightDirection_CameraSpace, eyeDirection_CameraSpace);

    EndPrimitive();

}

