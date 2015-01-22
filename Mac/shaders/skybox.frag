#version 330

in vec3 fNormal;
in vec2 fTexCoord;

out vec4 fColorOut;

uniform sampler2D tSampler;

void main()
{
    fColorOut = texture(tSampler, fTexCoord);
}

