#version 330

layout (location = 0) in float initRandIn;
layout (location = 1) in vec3  positionIn;
layout (location = 2) in vec3  velocityIn;
layout (location = 3) in vec3  colorIn;
layout (location = 4) in float fullLifeTimeIn;
layout (location = 5) in float actualLifeTimeIn;
layout (location = 6) in float sizeIn;
layout (location = 7) in float opacityIn;

out float initRand;
out vec3  position;
out vec3  velocity;
out vec3  color;
out float fullLifeTime;
out float actualLifeTime;
out float size;
out float opacity;

void main()
{
    initRand       = initRandIn;
    position       = positionIn;
    velocity       = velocityIn;
    color          = colorIn;
    fullLifeTime   = fullLifeTimeIn;
    actualLifeTime = actualLifeTimeIn;
    size           = sizeIn;
    opacity        = opacityIn;
}
