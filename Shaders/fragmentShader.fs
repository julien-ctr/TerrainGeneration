#version 330 core
out vec4 FragColor;

in vec2 texCoord;
<<<<<<< HEAD
=======
in vec4 outColor;
>>>>>>> c6f989f (Encapsulated camera related methods in a class + added ImGui to add a menu)

uniform sampler2D myTexture;

void main()
{
   // FragColor = texture(myTexture, texCoord);
<<<<<<< HEAD
   FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
=======
   FragColor = outColor;
>>>>>>> c6f989f (Encapsulated camera related methods in a class + added ImGui to add a menu)
};