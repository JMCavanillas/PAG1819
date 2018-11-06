#version 400

in vec3 tangent;

layout (location = 0) out vec4 fColor;

void main() {
	fColor = vec4(tangent.x, tangent.y, tangent.z, 1.0);
}