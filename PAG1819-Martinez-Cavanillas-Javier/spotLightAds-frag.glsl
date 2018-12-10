#version 400
#define M_PI 3.1415926535897932384626433832795

in vec3 position;
in vec3 normal;

uniform vec4 mtDiffuse;
uniform vec4 mtSpecular;
uniform float mtShininess;

uniform vec3 lgDiffuse;
uniform vec3 lgSpecular;
uniform vec3 lgPosition;
uniform vec3 lgDirection;
uniform float lgScattAng;
uniform float lgScattExp;

layout (location = 0) out vec4 fragColor;

vec3 diffuseAndSpecular()
{
	vec3 l = normalize(lgPosition - position);
	vec3 d = normalize(lgDirection);

	float cosGamma = cos(lgScattAng * M_PI / 180);
	float spotFactor = 1.0;

	if (dot(-l, d) < cosGamma) 
	{
		spotFactor = 0.0;
	} 
	else {
		spotFactor = pow(dot(-l, d), lgScattExp);
	}

	vec3 n = normalize(normal);
	vec3 v = normalize(-position);
	vec3 r = reflect(-l, n);

	vec3 diffuse = (lgDiffuse * vec3(mtDiffuse) * max(dot(l,n), 0.0));
	vec3 specular = (lgSpecular * vec3(mtSpecular) * pow(max(dot(r,v), 0.0), mtShininess));

	return spotFactor * (diffuse + specular);
}

void main() 
{
	fragColor = vec4(diffuseAndSpecular(), mtDiffuse.a);
}