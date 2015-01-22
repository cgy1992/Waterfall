#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;

uniform mat4 mvp;

out vec2 fTexCoord;

void main()
{
    gl_Position = mvp * vec4(vPosition, 1.0);
    fTexCoord = vTexCoord;
}

