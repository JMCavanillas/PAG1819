#version 400
layout (location = 0) in vec3 vPosition;
layout (location = 2) in vec3 vTangents;

uniform mat4 mModelViewProj;

out vec3 tangent;

void main() {
	gl_Position = mModelViewProj * vec4(vPosition, 1.0);
	tangent = vTangents;
}