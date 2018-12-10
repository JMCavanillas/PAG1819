#include "sceneObjects.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "MaterialLibrary.h"
#include "PAG3DGroup.h"
#include "PAGRevolutionObject.h"
#include "PAGPlane.h"

PAG3DElement * tie_fighter()
{
	std::vector<glm::vec2> submarine = { glm::vec2(0, 0), glm::vec2(0.8, 0), glm::vec2(4, 2),
		glm::vec2(4, 6), glm::vec2(0.8, 8), glm::vec2(0, 8) };
	std::vector<glm::vec2> hexagon = { glm::vec2(0, 0), glm::vec2(9, 0), glm::vec2(10, 0.5), 
		glm::vec2(10, 1), glm::vec2(9, 1), glm::vec2(0, 1) };
	std::vector<glm::vec2> cilinder = { glm::vec2(0, 0), glm::vec2(0.3, 0), glm::vec2(0.6, 0), 
		glm::vec2(0.6, 0.5), glm::vec2(0.6, 18), glm::vec2(0.6, 19.5), glm::vec2(0.3, 19.5), 
		glm::vec2(0, 19.5) };

	glm::mat4 helix_rot = glm::rotate(glm::mat4(1), glm::radians<float>(30), glm::vec3(0, 1, 0));

	glm::mat4 position = glm::rotate(glm::mat4(1), glm::pi<float>() / 2, glm::vec3(0, 0, 1));
	position = position * glm::translate(glm::mat4(1), glm::vec3(0, -10, 0));

	auto o_helix_1 = new PAGRevolutionObject(hexagon, 2, 6);
	auto o_helix_2 = new PAGRevolutionObject(hexagon, 2, 6);
	auto o_cilinder = new PAGRevolutionObject(cilinder, 2, 30);
	auto o_sphere = new PAGRevolutionObject(submarine, 2, 30);
	o_helix_1->setModelMatrix(position * helix_rot);
	o_sphere->setModelMatrix(position * glm::translate(glm::mat4(1), glm::vec3(0, 6, 0)));
	o_cilinder->setModelMatrix(position * glm::translate(glm::mat4(1), glm::vec3(0, 0.25, 0)));
	o_helix_2->setModelMatrix(position * glm::translate(glm::mat4(1), glm::vec3(0, 19, 0)) * helix_rot);

	o_helix_1->setMaterial(MaterialLibrary::getInstance()->getMaterial("crystal_black"));
	o_helix_2->setMaterial(MaterialLibrary::getInstance()->getMaterial("crystal_black"));

	o_sphere->setMaterial(MaterialLibrary::getInstance()->getMaterial("metal_grey"));
	o_cilinder->setMaterial(MaterialLibrary::getInstance()->getMaterial("metal_grey"));

	auto tie_fighter = new PAG3DGroup();
	tie_fighter->add(o_cilinder);
	tie_fighter->add(o_sphere);
	tie_fighter->add(o_helix_1);
	tie_fighter->add(o_helix_2);

	tie_fighter->setModelMatrix(glm::mat4(1));

	return tie_fighter;
}
