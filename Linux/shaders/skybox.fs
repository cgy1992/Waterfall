#version 330

in vec2 fTexCoord;

out vec4 fColorOut;

uniform sampler2D tSampler;

void main()
{
    fColorOut = texture(tSampler, fTexCoord);
    //fColorOut = vec4(1.0f, 1.0f, 0.0f, 1.f);
}

