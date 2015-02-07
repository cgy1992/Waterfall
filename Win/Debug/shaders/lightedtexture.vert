#version 330

layout (location = 0) in vec3 vPosition_ModelSpace;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec3 vTangent_ModelSpace;
layout (location = 3) in vec3 vBitangent_ModelSpace;
layout (location = 4) in vec3 vNormal_ModelSpace;

out vec2 texCoord;
out vec3 position_WorldSpace;

out vec3 lightDirection_TangentSpace;
out vec3 eyeDirection_TangentSpace;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform vec3 lightPosition_WorldSpace;

void main()
{
    gl_Position = MVP * vec4(vPosition_ModelSpace, 1.f);

    position_WorldSpace = (M * vec4(vPosition_ModelSpace, 1.f)).xyz;

    mat4 MV = V * M;
    mat3 MV3x3 = mat3(MV);

    vec3 vPosition_CameraSpace = (MV * vec4(vPosition_ModelSpace, 1.f)).xyz;
    vec3 lightPosition_CameraSpace = (V * vec4(lightPosition_WorldSpace, 1.f)).xyz;

    vec3 lightDirection_CameraSpace = lightPosition_CameraSpace - vPosition_CameraSpace;
    vec3 eyeDirection_CameraSpace = vec3(0, 0, 0) - vPosition_CameraSpace;

    vec3 vNormal_CameraSpace = MV3x3 * vNormal_ModelSpace;
    vec3 vTangent_CameraSpace = MV3x3 * vTangent_ModelSpace;
    vec3 vBitangent_CameraSpace = MV3x3 * vBitangent_ModelSpace;
    mat3 TBN = transpose(mat3(vTangent_CameraSpace, vBitangent_CameraSpace, vNormal_CameraSpace));

    lightDirection_TangentSpace = TBN * lightDirection_CameraSpace;
    eyeDirection_TangentSpace = TBN * eyeDirection_CameraSpace;
    texCoord = vTexCoord;
}