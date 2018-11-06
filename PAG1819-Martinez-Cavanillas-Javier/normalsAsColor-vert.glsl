#version 400
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 mModelViewProj;

out vec3 normal;

void main() {
	gl_Position = mModelViewProj * vec4(vPosition, 1.0);
	normal = vNormal;
}