#version 400
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vTangent;
layout (location = 3) in vec2 vTextCoords;

uniform mat4 mModelViewProj;

out vec3 normal;
out vec3 tangent;
out vec2 textCoords;

void main() {
	gl_Position = mModelViewProj * vec4(vPosition, 1.0);
	normal = vNormal;
	tangent = vTangent;
	textCoords = vTextCoords;
}