//#version 330 core
//out vec4 FragColor;
//
//void main()
//{
//    FragColor = vec4(0.04, 0.28, 0.26, 1.0);
//}

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
// texture samplers
uniform sampler2D textureInstance;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir),0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir),0.0),8);
    vec3 specular = specularStrength * spec * lightColor;

    // linearly interpolate between both textures (80% container, 20% awesomeface)
    FragColor =vec4(diffuse+ambient+specular,1.0)*texture(textureInstance, TexCoord);
	//FragColor = vec4(0.04, 0.28, 0.26, 1.0);
}