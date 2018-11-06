#version 400

in vec2 textCoords;

layout(location = 0) out vec4 fColor;

void main() {
	fColor = vec4(textCoords.x, 0, textCoords.y, 1.0);
}