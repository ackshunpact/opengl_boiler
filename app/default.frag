#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 uLightPos;
uniform vec3 uViewPos; 
uniform vec3 uLightColor;
uniform vec3 uObjectColor;

void main() {
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * uLightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(uLightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * uLightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(uViewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * uLightColor;

	vec3 result = (ambient + diffuse + specular) * uObjectColor;
	//FragColor = vec4(result, 1.0);
	FragColor = vec4(normalize(Normal) * 0.5 +0.5, 1.0);

}