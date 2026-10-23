#version 330 core

uniform sampler2D uGrass;
uniform sampler2D uGround;
uniform sampler2D uSnow;

uniform float uSnowHeight;
uniform float uBlend;

out vec4 FragColor;

in vec2 vUV;
in vec3 FragPos;
in vec3 Normal;
in vec3 vColor;

uniform vec3 uLightDir;
//uniform vec3 uLightPos;
uniform vec3 uViewPos; 
uniform vec3 uLightColor;
uniform vec3 uObjectColor;

//uniform sampler2D uAlbedo;
uniform vec4 uBaseColor;
uniform vec2 uUvScale;


void main() {

	
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * uLightColor;

	vec3 norm = normalize(Normal);
	vec2 uv = vUV * uUvScale;

	vec3 grass = texture(uGrass, uv).rgb;
	vec3 ground = texture(uGround, uv).rgb;
	vec3 snow= texture(uSnow, uv).rgb;
	float steep = 1.0 - smoothstep(0.7, 0.85, norm.y);
	float high= smoothstep(uSnowHeight - uBlend, uSnowHeight + uBlend, FragPos.y);
	vec3 grnd = mix(grass, snow, high);
	vec3 albedo = mix(grnd, ground, steep);

	vec3 toLight = normalize(uLightDir);
	float diff = max(dot(norm, toLight), 0.0f);
	vec3 diffuse = diff * uLightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(uViewPos - FragPos);
	vec3 reflectDir = reflect(-toLight, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * uLightColor;





	vec3 result = (ambient + diffuse) * albedo; // + specular;
	FragColor = vec4(result, 1.0);

}