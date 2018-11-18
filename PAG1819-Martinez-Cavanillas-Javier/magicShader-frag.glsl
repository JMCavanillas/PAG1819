#version 400

in vec3 normal;
in vec3 tangent;
in vec2 textCoords;

layout(location = 0) out vec4 fColor;

void main() {
	fColor = vec4(textCoords.x * tangent.z, normal.z, textCoords.y, 1.0);
}