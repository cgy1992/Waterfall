#version 330

in vec4 colorFrag;
in vec2 texPrevCoord;
in vec2 texNextCoord;
in vec2 texCoord; 
in float texNextSimilarity;
in vec3 lightDirection_TangentSpace;
in vec3 eyeDirection_TangentSpace;
in vec3 position_WorldSpace;

out vec4 colorFragOut;

uniform sampler2D tSamplerParticle;
uniform sampler2D tSamplerNormal;
uniform sampler2D tSamplerSpecular;
uniform sampler2D tSamplerBackground;
uniform sampler2D tSamplerRefract;

uniform float iScreenWidth;
uniform float iScreenHeight;

uniform vec3 lightPosition_WorldSpace;
uniform vec3 lightColor;
uniform float lightPower;

void main()
{
    /* Background */
    vec2 texBackgroundCoord = vec2(gl_FragCoord.x / iScreenWidth, gl_FragCoord.y / iScreenHeight);
    
    vec2 vScale = vec2(0.02, -0.03);
    vec2 vOffset = vec2(0.0, 0.0);
    vec4 texRefractCoord = texture(tSamplerRefract, texCoord);
    vec2 texScaledRefractCoord = texRefractCoord.xy * vScale;
    vec2 texScaledOffsetRefractCoord = texScaledRefractCoord + vOffset;
    
    vec4 backgroundRefractColor = texture(tSamplerBackground, texBackgroundCoord + texScaledOffsetRefractCoord);

    /* Particle */
    vec4 particleSourceColor = mix(texture(tSamplerParticle, texPrevCoord), texture(tSamplerParticle, texNextCoord), texNextSimilarity) * colorFrag;

    vec3 particleDiffuseColor = particleSourceColor.rgb;
    vec3 particleAmbientColor = particleDiffuseColor * vec3(0.2, 0.2, 0.2);
    vec3 particleSpecularColor = texture(tSamplerSpecular, texCoord).rgb * 0.3;

    vec3 normal_TangentSpace = normalize(texture(tSamplerNormal, texCoord).rgb * 2.0 - 1.0);

    vec3 n = normal_TangentSpace;
    vec3 l = normalize(lightDirection_TangentSpace);
    vec3 e = normalize(eyeDirection_TangentSpace);
    vec3 r = reflect(-l, n);

    float cosTheta = clamp(dot(l, n), 0, 1);
    float cosAlpha = clamp(dot(e, r), 0, 1);

    float distance = length(lightPosition_WorldSpace - position_WorldSpace);

    vec3 particleColor = vec3(0.f, 0.f, 0.f) 
                       + particleAmbientColor
                       + particleDiffuseColor * lightColor * cosTheta * lightPower / (distance * distance)
                       //+ particleSpecularColor * lightColor * lightPower * pow(cosAlpha, 5) / (distance * distance)
                       ;

    /* Result */
    colorFragOut = mix(backgroundRefractColor, vec4(particleColor, colorFrag.a), 0.3f);
    colorFragOut.a = particleSourceColor.a;
}
