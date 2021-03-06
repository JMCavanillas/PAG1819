#include <iostream>
#include <cstdlib>
// - IMPORTANTE: El include de Glew debe llamarse siempre ANTES de llamar al de GLFW.
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Windows.h>
#include "PAGRenderer.h"
#include "PAGEnumeration.h"

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

bool primary_mouse_bt_clicked = false;
bool secondary_mouse_bt_clicked = false;

double click_mouse_pos_x = 0;
double click_mouse_pos_y = 0;

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
	// std::cout << "Resize callback called" << std::endl;
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
	{
		PAGRenderer::getInstance()->zoom(3);
		PAGRenderer::getInstance()->refresh();
	}

	if (key == GLFW_KEY_SLASH && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->zoom(-3);
		PAGRenderer::getInstance()->refresh();
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->nextView();
		PAGRenderer::getInstance()->refresh();
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->setCamera(0);
		PAGRenderer::getInstance()->refresh();
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->setCamera(1);
		PAGRenderer::getInstance()->refresh();
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->setView(PAGDrawingMode::PAG_POINT_VIEW);
		PAGRenderer::getInstance()->refresh();
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->setView(PAGDrawingMode::PAG_LINE_VIEW);
		PAGRenderer::getInstance()->refresh();
	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->setView(PAGDrawingMode::PAG_FRONT_FACE_VIEW);
		PAGRenderer::getInstance()->refresh();
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->setView(PAGDrawingMode::PAG_POINT_LINE_TRIANGLE_VIEW);
		PAGRenderer::getInstance()->refresh();
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->pan(-5);
		PAGRenderer::getInstance()->refresh();
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->pan(5);
		PAGRenderer::getInstance()->refresh();
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->tilt(-5);
		PAGRenderer::getInstance()->refresh();
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->tilt(5);
		PAGRenderer::getInstance()->refresh();
	}

	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->truck(1);
		PAGRenderer::getInstance()->refresh();
	}
	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->truck(-1);
		PAGRenderer::getInstance()->refresh();
	}

	if (key == GLFW_KEY_U && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->boom(1);
		PAGRenderer::getInstance()->refresh();
	}
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->boom(-1);
		PAGRenderer::getInstance()->refresh();
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		PAGRenderer::getInstance()->setView(PAGDrawingMode::PAG_TRIANGLE_VIEW);
		PAGRenderer::getInstance()->refresh();
	}
}

// - Esta funci�n callback ser� llamada cada vez que se pulse alg�n bot�n
// del rat�n sobre el �rea de dibujo OpenGL.
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		PAGRenderer::getInstance()->button_clicked(button);
		if (button == 0)
			primary_mouse_bt_clicked = true;

		if (button == 1)
			secondary_mouse_bt_clicked = true;

		glfwGetCursorPos(window, &click_mouse_pos_x, &click_mouse_pos_y);
	}
	else if (action == GLFW_RELEASE) {
		PAGRenderer::getInstance()->button_released(button);
		primary_mouse_bt_clicked = false;
		secondary_mouse_bt_clicked = false;
	}
}

// - Esta funci�n callback ser� llamada cada vez que se mueva la rueda
// del rat�n sobre el �rea de dibujo OpenGL.
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
	if (yoffset > 0)
		PAGRenderer::getInstance()->dolly(-6);
	else
		PAGRenderer::getInstance()->dolly(6);

	PAGRenderer::getInstance()->refresh();
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
	int width;
	int height;
	glfwGetWindowSize(window,&width, &height);
	PAGRenderer::getInstance()->change_viewport_size(width, height);

	// - Ciclo de eventos de la aplicaci�n. La condici�n de parada es que la
	// ventana principal deba cerrarse, por ejemplo, si el usuario pulsa el
	// bot�n de cerrar la ventana (la X).
	while (!glfwWindowShouldClose(window)) {
		if (primary_mouse_bt_clicked) {
			double xpos, ypos;
			//getting cursor position
			glfwGetCursorPos(window, &xpos, &ypos);

			float displacement = (xpos / width) - (click_mouse_pos_x / width);

			PAGRenderer::getInstance()->orbit(20 * displacement);
			PAGRenderer::getInstance()->refresh();
		}
		if (secondary_mouse_bt_clicked) {
			double xpos, ypos;
			//getting cursor position
			glfwGetCursorPos(window, &xpos, &ypos);

			float displacement_x = (xpos / width) - (click_mouse_pos_x / width);
			float displacement_y = (ypos / height) - (click_mouse_pos_y / height);

			PAGRenderer::getInstance()->truck(8 * displacement_x);
			PAGRenderer::getInstance()->boom(-8 * displacement_y);
			PAGRenderer::getInstance()->refresh();
		}
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