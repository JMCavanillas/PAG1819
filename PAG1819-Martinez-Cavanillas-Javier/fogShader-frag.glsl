#version 400

in vec3 position;

uniform vec3 clearColor;
uniform float far;
uniform float near;

out vec4 fColor;

void main()
{
    float alpha = 0;
	if(length(position) > near){
		alpha = min((length(position) - near) / (far-near), 1);
	}

	fColor = vec4(clearColor, alpha);
}