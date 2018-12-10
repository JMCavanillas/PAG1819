#include "PAGRenderer.h"
#include "PAGRevolutionObject.h"
#include "PAGSubdivisionProfile.h"
#include "PAGPlane.h"
#include "MaterialLibrary.h"

#include "gtc/matrix_transform.hpp"

#include "sceneObjects.h"

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

	glm::mat4 vp = cameras_[currentCamera_].getPerspective() * cameras_[currentCamera_].getVision();
	glm::mat4 v = cameras_[currentCamera_].getVision();

	if (currentView_ == PAG_TRIANGLE_VIEW)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		for (int i = 0; i < lights_.size(); ++i)
		{
			if (i == 1)
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			PAGLightSource light = lights_[i];
			int shader_name = light.getPreferedShader();

			// Hay que obtener la referencia sin tomar la posesion del shader,
			// de lo contrario al final del bucle for se llama al destructor del
			// shader y lo elimina de la GPU
			const PAGShaderProgram* shader = &(shaders_[shader_name]);

			shader->use();
			light.setVision(v);
			light.apply(*shader);
			sceneObjects_.drawAsTriangles(*shader, vp, v);

		}

		// Fog
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		float far = cameras_[currentCamera_].getFar();
		float near = cameras_[currentCamera_].getNear();
		const PAGShaderProgram* shader = &(shaders_[PAG_FOG_SHADER]);
		shader->use();
		shader->setUniform("clearColor", glm::vec3(0.0f / 255.0f, 11.0f / 255.0f, 40.0f / 255.0f));
		shader->setUniform("near", float(near + (far - near) * 0.4));
		shader->setUniform("far", float(far - (far - near) * 0.2));
		sceneObjects_.drawAsTriangles(*shader, vp, v);
	}

	const PAGShaderProgram* shader = nullptr;
	switch (currentView_)
	{
	case PAG_POINT_VIEW:
		glDepthMask(GL_FALSE);
		shader = &(shaders_[PAG_POINT_SHADER]);
		shader->use();
		shader->setUniform("pointSize", 7.0f);
		shader->setUniform("vColor", glm::vec3(74.0f / 255.0f, 208.0f / 255.0f, 232.0f / 255.0f));
		sceneObjects_.drawAsPoints(*shader, vp, v);
		glDepthMask(GL_TRUE);
		break;
	case PAG_LINE_VIEW:
		shader = &(shaders_[PAG_LINE_SHADER]);
		shader->use();
		shader->setUniform("vColor", glm::vec3(74.0f / 255.0f, 208.0f / 255.0f, 232.0f / 255.0f));
		sceneObjects_.drawAsLines(*shader, vp, v);
		break;

	case PAG_POINT_LINE_TRIANGLE_VIEW:
		shader = &(shaders_[PAG_LINE_SHADER]);
		shader->use();
		shader->setUniform("vColor", glm::vec3(74.0f / 255.0f, 208.0f / 255.0f, 232.0f / 255.0f));
		sceneObjects_.drawAsLines(*shader, vp, v);

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);
		shader = &(shaders_[PAG_FRONT_FACE_SHADER]);
		shader->use();
		sceneObjects_.drawAsTriangles(*shader, vp, v);
		glDisable(GL_POLYGON_OFFSET_FILL);

		glDepthMask(GL_FALSE);
		shader = &(shaders_[PAG_POINT_SHADER]);
		shader->use();
		shader->setUniform("pointSize", 7.0f);
		shader->setUniform("vColor", glm::vec3(74.0f / 255.0f, 208.0f / 255.0f, 232.0f / 255.0f));
		sceneObjects_.drawAsPoints(*shader, vp, v);
		glDepthMask(GL_TRUE);
		break;
	case PAG_NORMALS_AS_COLORS_VIEW:
		shader = &(shaders_[PAG_NORMALS_AS_COLORS_SHADER]);
		shader->use();
		sceneObjects_.drawAsTriangles(*shader, vp, v);
		break;
	case PAG_TANGENTS_AS_COLORS_VIEW:
		shader = &(shaders_[PAG_TANGENTS_AS_COLORS_SHADER]);
		shader->use();
		sceneObjects_.drawAsTriangles(*shader, vp, v);
		break;
	case PAG_TEXTURE_COORDS_AS_COLORS_VIEW:
		shader = &(shaders_[PAG_TEXTURE_COORDS_AS_COLORS_SHADER]);
		shader->use();
		sceneObjects_.drawAsTriangles(*shader, vp, v);
		break;
	case PAG_MAGIC_VIEW:
		shader = &(shaders_[PAG_MAGIC_SHADER]);
		shader->use();
		sceneObjects_.drawAsTriangles(*shader, vp, v);
		break;
	case PAG_FRONT_FACE_VIEW:
		shader = &(shaders_[PAG_FRONT_FACE_SHADER]);
		shader->use();
		sceneObjects_.drawAsTriangles(*shader, vp, v);
		break;
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
	
	for (PAGCamera& camera : cameras_)
		camera.setViewport(width, height);

}

void PAGRenderer::prepareOpenGL()
{
	currentView_ = PAGDrawingMode::PAG_TRIANGLE_VIEW;
	// - Crea el shader program que dibujará la geometría en forma de nube de puntos.
	shaders_.createShaderProgram(PAG_POINT_SHADER, "pointShader");
	shaders_.createShaderProgram(PAG_LINE_SHADER, "lineShader");
	shaders_.createShaderProgram(PAG_FRONT_FACE_SHADER, "triangleFaceShader");
	shaders_.createShaderProgram(PAG_NORMALS_AS_COLORS_SHADER, "normalsAsColor");
	shaders_.createShaderProgram(PAG_TANGENTS_AS_COLORS_SHADER, "tangentsAsColor");
	shaders_.createShaderProgram(PAG_TEXTURE_COORDS_AS_COLORS_SHADER, "textCoordsAsColors");
	shaders_.createShaderProgram(PAG_MAGIC_SHADER, "magicShader");
	shaders_.createShaderProgram(PAG_POINTLIGHT_SHADER, "pointLightAds");
	shaders_.createShaderProgram(PAG_AMBIENTLIGHT_SHADER, "ambientLightAds");
	shaders_.createShaderProgram(PAG_DIRLIGHT_SHADER, "dirLightAds");
	shaders_.createShaderProgram(PAG_SPOTLIGHT_SHADER, "spotLightAds");
	shaders_.createShaderProgram(PAG_FOG_SHADER, "fogShader");

	// - Establecemos un gris medio como color con el que se borrará el frame buffer.
	// No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
	glClearColor(0. / 255, 11. / 255, 40. / 255, 1.0);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);

	// - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
	// No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Generamos objetos de la escena
	// std::vector<glm::vec2> UFO = { glm::vec2(0, 0), glm::vec2(10, 1), glm::vec2(10, 3), glm::vec2(2, 4), glm::vec2(0, 6) };

	glm::mat4 position = glm::mat4(1);
	auto tie_fighter_1 = tie_fighter();
	position = glm::translate(glm::mat4(1), glm::vec3(0, 10, 0));
	tie_fighter_1->setModelMatrix(position);
	auto tie_fighter_2 = tie_fighter();
	position = glm::translate(glm::mat4(1), glm::vec3(30, 20, -60)) *
		glm::rotate(glm::mat4(1), glm::radians<float>(-30), glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1), glm::radians<float>(-12), glm::vec3(0, 0, 1));
	tie_fighter_2->setModelMatrix(position);
	tie_fighter_2->setMaterial(MaterialLibrary::getInstance()->getMaterial("green"));
	auto tie_fighter_3 = tie_fighter();
	position = glm::translate(glm::mat4(1), glm::vec3(-30, 20, 20)) *
		glm::rotate(glm::mat4(1), glm::radians<float>(30), glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1), glm::radians<float>(20), glm::vec3(0, 0, 1));
	tie_fighter_3->setModelMatrix(position);
	tie_fighter_3->setMaterial(MaterialLibrary::getInstance()->getMaterial("red"));
	sceneObjects_.add(tie_fighter_1);
	sceneObjects_.add(tie_fighter_2);
	sceneObjects_.add(tie_fighter_3);

	auto object_2 = new PAGPlane(500, 500, 50, 50);
	object_2->setMaterial(MaterialLibrary::getInstance()->getMaterial("white"));
	object_2->setModelMatrix(glm::translate(glm::mat4(1), glm::vec3(-250, 0, -250)));
	sceneObjects_.add(object_2);

	// Definimos las luces
	PAGLightSource light2;
	light2.setAmbient(glm::vec3(0.08, 0.08, 0.08));
	light2.setApplier(&ambientLightApplier_);
	lights_.push_back(light2);

	PAGLightSource light4;
	light4.setDirection(glm::vec3(-1, -0.5, -1));
	light4.setApplier(&dirLightApplier_);
	lights_.push_back(light4);

	PAGLightSource light3;
	light3.setPosition(glm::vec3(-25, 10, -100));
	light3.setApplier(&pointLightApplier_);
	lights_.push_back(light3);

	PAGLightSource light1;
	light1.setPosition(glm::vec3(0, 10, 0));
	light1.setDiffuse(glm::vec3(0.3, 0.3, 0.3));
	light1.setSpecular(glm::vec3(0.3, 0.3, 0.3));
	light1.setDirection(glm::vec3(0, -1, 0.7));
	light1.setScatterAngle(20);
	light1.setApplier(&spotLightApplier_);
	lights_.push_back(light1);

	// Establecemos cámaras
	currentCamera_ = 0;
	cameras_.push_back(PAGCamera(glm::vec3(30, 30, 85), glm::vec3(0, 7, 0), 1, 400, 60));
	cameras_.push_back(PAGCamera(glm::vec3(0,80,0), glm::vec3(0,5,0), 1, 400, 60));
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

void PAGRenderer::tilt(float degrees)
{
	cameras_[currentCamera_].tilt(degrees);
}

void PAGRenderer::pan(float degrees)
{
	cameras_[currentCamera_].pan(degrees);
}

void PAGRenderer::truck(float distance)
{
	cameras_[currentCamera_].truck(distance);
}

void PAGRenderer::boom(float distance)
{
	cameras_[currentCamera_].boom(distance);
}

void PAGRenderer::dolly(float distance)
{
	cameras_[currentCamera_].dolly(distance);
}

void PAGRenderer::orbit(float degrees)
{
	cameras_[currentCamera_].orbit_y(degrees);
}

void PAGRenderer::zoom(int factor)
{
	cameras_[currentCamera_].zoom(factor);
}


PAGRenderer::~PAGRenderer()
{
}
