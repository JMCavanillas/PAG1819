#version 400

layout (location = 0) out vec4 fColor;

void main() {
	// Si el fragmento pertenece a una primitiva
	// vista desde fuera se dibuja de color
	// verde y si pertenece a una vista desde dentro
	//se dibuja en rojo
	if (gl_FrontFacing) 
	{
		fColor = vec4(0.0, 1.0, 0.0, 1.0);
	} else {
		fColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
}