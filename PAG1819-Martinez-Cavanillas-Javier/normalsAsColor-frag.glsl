#version 400

in vec3 normal;

layout (location = 0) out vec4 fColor;

void main() {
	fColor = vec4(normal.x, normal.y, normal.z, 1.0);
}