#version 330

in vec3 fNormal;
in vec2 fTexCoord;

out vec4 fColorOut;

uniform sampler2D tSampler;

void main()
{
    fColorOut = texture(tSampler, fTexCoord);
//    fColorOut = vec4(1.f, 0.f, 0.f, 1.f);
}