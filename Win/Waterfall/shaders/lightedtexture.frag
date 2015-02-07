#version 330

in vec2 texCoord;
in vec3 position_WorldSpace;

in vec3 lightDirection_TangentSpace;
in vec3 eyeDirection_TangentSpace;

out vec4 colorOut;

uniform sampler2D tSamplerDiffuse;
uniform sampler2D tSamplerNormal;
uniform sampler2D tSamplerSpecular;

uniform vec3 lightPosition_WorldSpace;
uniform vec3 lightColor;
uniform float lightPower;

void main()
{
    vec3 materialDiffuseColor = texture(tSamplerDiffuse, texCoord).rgb;
    vec3 materialAmbientColor = vec3(0.1, 0.1, 0.1) * materialDiffuseColor;
    vec3 materialSpecularColor = texture(tSamplerSpecular, texCoord).rgb * 0.3;

    vec3 normal_TangentSpace = normalize(texture(tSamplerNormal, texCoord).rgb * 2.0 - 1.0);

    float distance = length(lightPosition_WorldSpace - position_WorldSpace);

    vec3 n = normal_TangentSpace;
    vec3 l = normalize(lightDirection_TangentSpace);
    float cosTheta = clamp(dot(n,l), 0, 1);

    vec3 e = normalize(eyeDirection_TangentSpace);
    vec3 r = reflect(-l, n);
    float cosAlpha = clamp(dot(e, r), 0, 1);

    vec3 color = materialAmbientColor /* ambient color */
               + materialDiffuseColor  * lightColor * lightPower * cosTheta         / (distance * distance) /* diffuse color */
               //+ materialSpecularColor * lightColor * lightPower * pow(cosAlpha, 5) / (distance * distance) /* specular color */
            ;

    colorOut = vec4(color, 1.f);
}

