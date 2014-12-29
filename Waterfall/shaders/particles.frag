#version 330

in vec4  colorFrag;
in vec2  texPrevCoord;
in vec2  texNextCoord;
in float texNextSimilarity;

uniform sampler2D tSampler;

out vec4 colorFragOut;

void main()
{
    vec4 textureMix = mix(texture(tSampler, texPrevCoord), texture(tSampler, texNextCoord), texNextSimilarity);
    colorFragOut = textureMix * colorFrag; //vec4(1.0, 1.0, 1.0, 1.0);
}
