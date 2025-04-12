#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec4 outColor;
in vec3 fragPos;
in vec3 normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D myTexture;

void main()
{
	vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);

    // Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

    // Specular shading (Blinn-Phong)
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    // Combine results
    vec3 ambient = 0.1 * vec3(1.0, 1.0, 1.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    vec3 specular = spec * vec3(1.0, 1.0, 1.0);

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result * outColor.rgb, 1.0);
};