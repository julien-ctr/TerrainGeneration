#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D myTexture;

void main()
{
   // FragColor = texture(myTexture, texCoord);
   FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
};