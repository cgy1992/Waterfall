#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 40) out;

in float initRand[];
in vec3 velocity[];
in vec3 color[];
in float fullLifeTime[];
in float actualLifeTime[];
in float size[];
in float opacity[];

out vec4 colorFragIn;
out vec2 texCoord;

uniform int texRowCount;
uniform int texColumnCount;

uniform mat4 mView;
uniform mat4 mProj;

uniform vec3 quad1;
uniform vec3 quad2;

void main()
{
    vec3 posCenter = gl_in[0].gl_Position.xyz;
    mat4 mVP = mProj * mView;
    float relativeLifeTime = actualLifeTime[0] / fullLifeTime[0];
    int texCount = texRowCount * texColumnCount;
    int texNum = int(floor(relativeLifeTime * texCount));
    int texRow = texNum / texColumnCount;
    int texColumn = texNum % texColumnCount;

    colorFragIn = vec4(color[0], opacity[0]);

    vec3 posLeftBottom = posCenter + (-quad1 - quad2) * size[0];
    texCoord = vec2(1.0 * texColumn / texColumnCount, 1.0 * texRow / texRowCount); //vec2(0.0f, 0.0f); //
    gl_Position = mVP * vec4(posLeftBottom, 1.0);  //(vec4(posCenter, 1.0) + vec4(0.0, 0.0, 0.0, 1.0)); //
    EmitVertex();

    vec3 posLeftTop = posCenter + (-quad1 + quad2) * size[0];
    texCoord = vec2(1.0 * texColumn / texColumnCount, 1.0 * (texRow + 1) / texRowCount); //vec2(0.0f, 1.0f); //
    gl_Position = mVP * vec4(posLeftTop, 1.0); //(vec4(posCenter, 1.0) + vec4(0.0, 1.0, 0.0, 1.0)); //
    EmitVertex();

    vec3 posRightBottom = posCenter + (quad1 - quad2) * size[0];
    texCoord = vec2(1.0 * (texColumn + 1) / texColumnCount, 1.0 * texRow / texRowCount); //vec2(1.0f, 0.0f); //
    gl_Position = mVP * vec4(posRightBottom, 1.0); //(vec4(posCenter, 1.0) + vec4(1.0, 0.0, 0.0, 1.0)); //
    EmitVertex();

    vec3 posRightTop = posCenter + (quad1 + quad2) * size[0];
    texCoord = vec2(1.0 * (texColumn + 1) / texColumnCount, 1.0 * (texRow + 1) / texRowCount);//vec2(1.0f, 1.0f); //
    gl_Position = mVP * vec4(posRightTop, 1.0); //(vec4(posCenter, 1.0) + vec4(1.0, 1.0, 0.0, 1.0)); //
    EmitVertex();

    EndPrimitive();
}
