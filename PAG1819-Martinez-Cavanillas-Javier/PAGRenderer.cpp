#include "PAGRenderer.h"
#include "PAGRevolutionObject.h"
#include "PAGSubdivisionProfile.h"

#include "gtc/matrix_transform.hpp"

PAGRenderer* PAGRenderer::instance_ = nullptr;

PAGRenderer* PAGRenderer::getInstance()
{
	if (!instance_)
		instance_ = new PAGRenderer();
	
	return instance_;
	
}

PAGRenderer::PAGRenderer()
{
	viewport_width = 1;
	viewport_height = 1;
}

void PAGRenderer::refresh()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 m = glm::perspective(glm::radians(20.0f), float(viewport_width) / float(viewport_height), 1.0f, 300.0f) * cameras_[currentCamera_];

	switch (currentView_)
	{
	case PAG_POINT_VIEW:
		glDepthMask(GL_FALSE);
		pointShaderConfig(7.0f, glm::vec3(74.0f / 255.0f, 208.0f / 255.0f, 232.0f / 255.0f), m);
		for (PAGRevolutionObject object : sceneObjects_)
			object.drawAsPoints();

		glDepthMask(GL_TRUE);
		break;
	case PAG_LINE_VIEW:
		lineShaderConfig(glm::vec3(74.0f / 255.0f, 208.0f / 255.0f, 232.0f / 255.0f), m);
		for (PAGRevolutionObject object : sceneObjects_)
			object.drawAsLines();
		break;
	case PAG_TRIANGLE_VIEW:
		lineShaderConfig(glm::vec3(101.0f / 255.0f, 101.0f / 255.0f, 120.0f / 255.0f), m);
		for (PAGRevolutionObject object : sceneObjects_)
			object.drawAsTriangles();
		break;
	case PAG_POINT_LINE_TRIANGLE_VIEW:
		lineShaderConfig(glm::vec3(55.0f / 255.0f, 125.0f / 255.0f, 185.0f / 255.0f), m);
		for (PAGRevolutionObject object : sceneObjects_)
			object.drawAsLines();
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);
		triangleFaceShaderConfig(m);
		for (PAGRevolutionObject object : sceneObjects_)
			object.drawAsTriangles();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDepthMask(GL_FALSE);
		pointShaderConfig(13.0f, glm::vec3(55.0f / 255.0f, 125.0f / 255.0f, 185.0f / 255.0f), m);
		for (PAGRevolutionObject object : sceneObjects_)
			object.drawAsPoints();

		glDepthMask(GL_TRUE);
		break;
	case PAG_FRONT_FACE_VIEW:
		triangleFaceShaderConfig(m);
		for (PAGRevolutionObject object : sceneObjects_)
			object.drawAsTriangles();
		break;
	case PAG_NORMALS_AS_COLORS_VIEW:
		normalsAsColorShaderConfig(m);
		for (PAGRevolutionObject object : sceneObjects_)
			object.drawAsTriangles();
		break;
	case PAG_TANGENTS_AS_COLORS_VIEW:
		tangentsAsColorShaderConfig(m);
		for (PAGRevolutionObject object : sceneObjects_)
			object.drawAsTriangles();
		break;
	case PAG_TEXTURE_COORDS_AS_COLORS_VIEW:
		textCoordsAsColorsShaderConfig(m);
		for (PAGRevolutionObject object : sceneObjects_)
			object.drawAsTriangles();
		break;
	case PAG_MAGIC_VIEW:
		magicShaderConfig(m);
		for (PAGRevolutionObject object : sceneObjects_)
			object.drawAsTriangles();
	default:
		break;
	}



	// std::cout << "Refresh callback called" << std::endl;
}

void PAGRenderer::change_viewport_size(int width, int height)
{
	viewport_height = height;
	viewport_width = width;
	glViewport(0, 0, width, height);
}

void PAGRenderer::prepareOpenGL()
{
	currentView_ = PAGDrawingMode::PAG_POINT_VIEW;
	// - Crea el shader program que dibujará la geometría en forma de nube de puntos.
	pointShader_.createShaderProgram("pointShader");
	lineShader_.createShaderProgram("lineShader");
	triangleFaceShader_.createShaderProgram("triangleFaceShader");
	normalsAsColorShader_.createShaderProgram("normalsAsColor");
	tangentsAsColorShader_.createShaderProgram("tangentsAsColor");
	textCoordsAsColorShader_.createShaderProgram("textCoordsAsColors");
	magicShader_.createShaderProgram("magicShader");

	// - Establecemos un gris medio como color con el que se borrará el frame buffer.
	// No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
	glClearColor(10.0f/255.0f, 16.0f / 255.0f, 22.0f / 255.0f, 1.0);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
	// No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
	glEnable(GL_DEPTH_TEST);

	// Generamos objetos de la escena
	std::vector<glm::vec2> points = { glm::vec2(0, 0), glm::vec2(10, 1), glm::vec2(10, 3), glm::vec2(2, 4), glm::vec2(0, 6) };
	auto object_1 = PAGRevolutionObject(points, 3, 30);

	sceneObjects_.push_back(object_1);

	// Establecemos cámaras
	currentCamera_ = 0;
	cameras_.push_back(glm::lookAt(glm::vec3(0, 10, 35), glm::vec3(0, 3, 0), glm::vec3(0, 1, 0)));
	cameras_.push_back(glm::lookAt(glm::vec3(0, 14, 0), glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)));
}

void PAGRenderer::pointShaderConfig(float point_size, const glm::vec3& color, const glm::mat4& view_matrix)
{
	pointShader_.use();
	pointShader_.setUniform("pointSize", point_size);
	pointShader_.setUniform("vColor", color);
	pointShader_.setUniform("mModelViewProj", view_matrix);
}

void PAGRenderer::lineShaderConfig(const glm::vec3 & color, const glm::mat4 & view_matrix)
{
	lineShader_.use();
	lineShader_.setUniform("vColor", color);
	lineShader_.setUniform("mModelViewProj", view_matrix);
}

void PAGRenderer::triangleFaceShaderConfig(const glm::mat4 & view_matrix)
{
	triangleFaceShader_.use();
	triangleFaceShader_.setUniform("mModelViewProj", view_matrix);
}

void PAGRenderer::normalsAsColorShaderConfig(const glm::mat4 & view_matrix)
{
	normalsAsColorShader_.use();
	normalsAsColorShader_.setUniform("mModelViewProj", view_matrix);
}

void PAGRenderer::tangentsAsColorShaderConfig(const glm::mat4 & view_matrix)
{
	tangentsAsColorShader_.use();
	tangentsAsColorShader_.setUniform("mModelViewProj", view_matrix);
}

void PAGRenderer::textCoordsAsColorsShaderConfig(const glm::mat4 & view_matrix)
{
	textCoordsAsColorShader_.use();
	textCoordsAsColorShader_.setUniform("mModelViewProj", view_matrix);
}

void PAGRenderer::magicShaderConfig(const glm::mat4 & view_matrix)
{
	magicShader_.use();
	magicShader_.setUniform("mModelViewProj", view_matrix);
}

void PAGRenderer::nextView()
{
	currentView_ = (currentView_ + 1) % 9;
}

void PAGRenderer::nextCamera()
{
	currentCamera_ = (currentCamera_ + 1) % cameras_.size();
}

void PAGRenderer::setCamera(unsigned n_camera)
{
	currentCamera_ = n_camera % cameras_.size();
}

void PAGRenderer::setView(int view)
{
	currentView_ = view % 9;
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
