#version 400

in vec4 destinationColor;
out vec4 fColor;

void main() {
	vec2 coord = gl_PointCoord - vec2(0.5);
	if (length(coord) > 0.5)
	{
		discard;
	} else if (length(coord) > 0.4) {
		fColor = destinationColor;
		fColor.a = (0.5 - length(coord)) / (0.5 - 0.4);
	} else {
		fColor = destinationColor;
	}
}
	
