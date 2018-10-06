#include "PAGRenderer.h"

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
