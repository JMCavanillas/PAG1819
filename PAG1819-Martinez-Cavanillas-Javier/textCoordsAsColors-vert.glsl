#version 400
layout (location = 0) in vec3 vPosition;
layout (location = 3) in vec2 vTextCoords;

uniform mat4 mModelViewProj;

out vec2 textCoords;

void main() {
	gl_Position = mModelViewProj * vec4(vPosition, 1.0);
	textCoords = vTextCoords;
}