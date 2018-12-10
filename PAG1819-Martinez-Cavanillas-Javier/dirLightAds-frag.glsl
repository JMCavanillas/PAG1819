#version 400

in vec3 position;
in vec3 normal;

uniform vec4 mtDiffuse;
uniform vec4 mtSpecular;
uniform float mtShininess;

uniform vec3 lgDiffuse;
uniform vec3 lgSpecular;
uniform vec3 lgDirection;

layout (location = 0) out vec4 fragColor;

vec3 diffuseAndSpecular()
{
	vec3 n = normalize (normal);
	
	vec3 l = -lgDirection;
	vec3 v = normalize(-position);
	vec3 r = reflect(-l, n);

	vec3 diffuse = (lgDiffuse * vec3(mtDiffuse) * max(dot(l,v), 0.0));
	vec3 specular = (lgSpecular * vec3(mtSpecular) * pow(max(dot(r,v), 0.0), mtShininess));

	return diffuse + specular;
}

void main() 
{
	fragColor = vec4(diffuseAndSpecular(), mtDiffuse.a);
}