#include <iostream>
#include <cstdlib>
// - IMPORTANTE: El include de Glew debe llamarse siempre ANTES de llamar al de GLFW.
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Windows.h>
#include "PAGRenderer.h"

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}


// - Esta funci�n callback ser� llamada cada vez que el �rea de dibujo
// OpenGL deba ser redibujada.
void window_refresh_callback(GLFWwindow *window) {
	PAGRenderer::getInstance()->refresh();
	// - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
	// intercambia el buffer back (que se ha estado dibujando) por el
	// que se mostraba hasta ahora front. Debe ser la �ltima orden de
	// este callback
	glfwSwapBuffers(window);
}

// - Esta funci�n callback ser� llamada cada vez que se cambie el tama�o
// del �rea de dibujo OpenGL.
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {

	PAGRenderer::getInstance()->change_viewport_size(width, height);
	std::cout << "Resize callback called" << std::endl;
}

// - Esta funci�n callback ser� llamada cada vez que se pulse una tecla
// dirigida al �rea de dibujo OpenGL.
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	// La eleccion del comportamiento del teclado le corresponde a GLFW a nivel de ventana por 
	// lo que llamar� a la funci�n necesaria de PAGRenderer solo si es necesario porque la 
	// tecla pulsada tenga que ver con el redibujo de escena
	if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS)
		PAGRenderer::getInstance()->zoom(3);

	if (key == GLFW_KEY_SLASH && action == GLFW_PRESS)
		PAGRenderer::getInstance()->zoom(-3);

	std::cout << "Key callback called, Key " << key << std::endl;
}

// - Esta funci�n callback ser� llamada cada vez que se pulse alg�n bot�n
// del rat�n sobre el �rea de dibujo OpenGL.
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		PAGRenderer::getInstance()->button_clicked(button);
	}
	else if (action == GLFW_RELEASE) {
		PAGRenderer::getInstance()->button_released(button);
	}
}

// - Esta funci�n callback ser� llamada cada vez que se mueva la rueda
// del rat�n sobre el �rea de dibujo OpenGL.
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
	if (yoffset > 0)
		PAGRenderer::getInstance()->zoom(3);
	else
		PAGRenderer::getInstance()->zoom(-3);

	std::cout << "Movida la rueda del raton " << xoffset <<
		" Unidades en horizontal y " << yoffset << " unidades en vertical" << std::endl;
}

int main() {
	std::cout << "Starting application pag prueba 01" << std::endl;
	// - Inicializar GLFW. Es un proceso que s�lo debe realizarse una vez.
	if (glfwInit() != GLFW_TRUE) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	// - Definimos las caracter�sticas que queremos que tenga nuestro contexto gr�fico
	// OpenGL. Por ejemplo, el n�mero de muestras o modo Core Profile.
	glfwWindowHint(GLFW_SAMPLES, 4); // - Activa antialiasing con 4 muestras.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // - Esta y las
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // siguientes activan un contexto
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // OpenGL Core Profile 4.1.
	// - Definimos la ventana de la aplicaci�n y la creamos
	GLFWwindow *window;

	// - Tama�o, t�tulo de la ventana, en ventana y no en pantalla completa,
	// sin compartir recursos con otras ventanas.
	window = glfwCreateWindow(1024, 576, "Pag practice", nullptr, nullptr);
	// - Comprobamos si la creaci�n de la ventana ha tenido �xito.
	if (window == nullptr) {
		std::cout << "Failed to open GLFW window" << std::endl;
		glfwTerminate(); // - Liberamos los recursos que ocupaba GLFW.
		return -2;
	}
	// - Hace que la ventana que se pasa como argumento use el contexto
	// de GLFW cuyas caracter�sticas se han definido previamente.
	glfwMakeContextCurrent(window);
	// - Ahora inicializamos GLEW.
	// IMPORTANTE: Glew debe inicializarse siempre DESPU�S de que se haya
	// inicializado y creado GLFW.
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		glfwTerminate(); // - Liberamos los recursos que ocupaba GLFW.
		return -3;
	}

	// - Interrogamos a OpenGL para que nos informe de las propiedades del contexto
	// 3D construido.
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VENDOR) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// - Registramos los callbacks que responder�n a los eventos principales
	glfwSetWindowRefreshCallback(window, window_refresh_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Realizamos configuraciones iniciales de la escena
	PAGRenderer::getInstance()->prepareOpenGL();

	// - Ciclo de eventos de la aplicaci�n. La condici�n de parada es que la
	// ventana principal deba cerrarse, por ejemplo, si el usuario pulsa el
	// bot�n de cerrar la ventana (la X).
	while (!glfwWindowShouldClose(window)) {
		// - Borrar los buffers (color y profundidad)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
		// intercambia el buffer back (que se ha estado dibujando) por el
		// que se mostraba hasta ahora front.
		glfwSwapBuffers(window);
		// - Obtiene y organiza los eventos pendientes, tales como pulsaciones de
		// teclas o de rat�n, etc. Siempre al final de cada iteraci�n del ciclo de
		// eventos y despu�s de glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// - Una vez terminado el ciclo de eventos, liberar recursos, etc.
	std::cout << "Finishing application pag prueba" << std::endl;
	glfwDestroyWindow(window); // - Cerramos y destruimos la ventana de la aplicaci�n.
	glfwTerminate(); // - Liberamos los recursos que ocupaba GLFW.
	// - Esta llamada es para impedir que la consola se cierre inmediatamente tras la
	// ejecuci�n y poder leer los mensajes. Se puede usar tambi�n getChar();
	system("pause");
}