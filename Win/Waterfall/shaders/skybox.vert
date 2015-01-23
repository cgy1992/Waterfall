#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

out vec3 fNormal;
out vec2 fTexCoord;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(vPosition, 1.f);
    fNormal = vNormal;
    fTexCoord = vTexCoord;
}

