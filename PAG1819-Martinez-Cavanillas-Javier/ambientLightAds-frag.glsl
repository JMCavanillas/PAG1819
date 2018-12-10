#version 400

uniform vec3 lgAmbient;

layout (location = 0) out vec4 fragColor;

void main()
{
	fragColor = vec4(lgAmbient, 1);
}