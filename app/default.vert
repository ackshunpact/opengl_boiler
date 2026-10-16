#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in mat4 aModel;
layout (location = 7) in vec4 aColor;

out vec3 vColor;

uniform mat4 uView;
uniform mat4 uProjection;

out vec3 FragPos;
out vec3 Normal;


void main() {
	vColor = aColor.rgb;
	FragPos = vec3(aModel * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(aModel))) * aNormal;
	gl_Position = uProjection * uView * aModel * vec4(aPos, 1.0); 
	
}
