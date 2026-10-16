#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 vColor;

uniform vec3 uLightDir;
//uniform vec3 uLightPos;
uniform vec3 uViewPos; 
uniform vec3 uLightColor;
uniform vec3 uObjectColor;

void main() {


	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * uLightColor;

	vec3 norm = normalize(Normal);
	//vec3 lightDir = normalize(uLightPos - FragPos);
	vec3 toLight = normalize(uLightDir);
	float diff = max(dot(norm, toLight), 0.0f);
	vec3 diffuse = diff * uLightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(uViewPos - FragPos);
	vec3 reflectDir = reflect(-toLight, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * uLightColor;

	vec3 result = (ambient + diffuse) * vColor + specular;
	FragColor = vec4(result, 1.0);
	

}