#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;
out vec4 outColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 top = vec3(0.0, 0.3, 0.0);
vec3 bottom = vec3(0.0, 0.6, 0.0);

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    outColor = vec4(mix(top, bottom, aPos.y / 2.0), 1.0);
    texCoord = aTexCoord;
};