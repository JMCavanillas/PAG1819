#include "PAGRenderer.h"
#include "PAGRevolutionObject.h"
#include "PAGSubdivisionProfile.h"

PAGRenderer* PAGRenderer::instance_ = nullptr;

PAGRenderer* PAGRenderer::getInstance()
{
	if (!instance_)
		instance_ = new PAGRenderer();
	
	return instance_;
	
}

PAGRenderer::PAGRenderer()
{
}

void PAGRenderer::refresh()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	std::cout << "Refresh callback called" << std::endl;
}

void PAGRenderer::change_viewport_size(int width, int height)
{
	glViewport(0, 0, width, height);
}

void PAGRenderer::prepareOpenGL()
{
	// - Establecemos un gris medio como color con el que se borrará el frame buffer.
	// No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
	glClearColor(0.6, 0.6, 0.6, 1.0);
	// - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
	// No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
	glEnable(GL_DEPTH_TEST);

	std::vector<glm::vec2> points = {glm::vec2(0, 0), glm::vec2(10, 1), glm::vec2(10, 3), glm::vec2(2, 4), glm::vec2(0, 6) };

	PAGRevolutionObject object_1(points, 2, 9);
	// holo.exportPosNorm(PAGRevObjParts::PAG_BODY);

}

void PAGRenderer::button_clicked(int button)
{
	std::cout << "Pulsado el boton: " << button << std::endl;
}

void PAGRenderer::button_released(int button)
{
	std::cout << "Soltado el boton: " << button << std::endl;
}

void PAGRenderer::zoom(int cantidad)
{
	std::cout << "Zoom: " << cantidad << std::endl;
}


PAGRenderer::~PAGRenderer()
{
}
