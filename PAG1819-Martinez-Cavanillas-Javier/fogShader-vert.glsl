#version 400

layout (location = 0) in vec3 vPosition;

uniform mat4 mModelViewProj;
uniform mat4 mModelView;

out vec3 position;

void main()
{
	position = vec3(mModelView * vec4(vPosition, 1.0));
	gl_Position = mModelViewProj * vec4(vPosition, 1.0);
}