#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec4 outColor;

uniform sampler2D myTexture;

void main()
{
   // FragColor = texture(myTexture, texCoord);
   FragColor = outColor;
};