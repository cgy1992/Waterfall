#version 330

in vec4  colorFrag;
in vec2  texPrevCoord;
in vec2  texNextCoord;
in float texNextSimilarity;

uniform sampler2D tSamplerParticle;
uniform sampler2D tSamplerBackground;
uniform sampler2D tSamplerRefract;

uniform int iScreenWidth;
uniform int iScreenHeight;

out vec4 colorFragOut;

void main()
{
    vec2 texBackgroundCoord = vec2(gl_FragCoord.x / iScreenWidth, gl_FragCoord.y / iScreenHeight);
    
    vec2 vScale = vec2(0.02, -0.03);
    vec2 vOffset = vec2(0.0, 0.0);
    vec4 texRefractCoord = texture(tSamplerRefract, texPrevCoord);
    vec2 texScaledRefractCoord = texRefractCoord.xy * vScale;
    vec2 texScaledOffsetRefractCoord = texScaledRefractCoord + vOffset;
    
    vec4 backgroundRefractColor = texture(tSamplerBackground, texBackgroundCoord + texScaledOffsetRefractCoord);
    
    vec4 particleColor = mix(texture(tSamplerParticle, texPrevCoord), texture(tSamplerParticle, texNextCoord), texNextSimilarity);
    particleColor = particleColor * colorFrag;
    
    colorFragOut = mix(backgroundRefractColor, particleColor, 0.7f);
    colorFragOut.a = particleColor.a;
}
