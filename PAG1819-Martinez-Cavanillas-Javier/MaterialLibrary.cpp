#include "MaterialLibrary.h"

MaterialLibrary* MaterialLibrary::instance_ = nullptr;

MaterialLibrary::MaterialLibrary()
{
	// Materiales Predefinidos
	this->setMaterial("red", PAGMaterial(glm::vec4(1,0,0,1), glm::vec4(0, 0, 0, 1), 1));
	this->setMaterial("white", PAGMaterial(glm::vec4(1, 1, 1, 1), glm::vec4(0, 0, 0, 1), 1));
	this->setMaterial("black", PAGMaterial(glm::vec4(0.1, 0.1, 0.1, 1), glm::vec4(0.1, 0.1, 0.1, 1), 1));
	this->setMaterial("blue", PAGMaterial(glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 1, 1), 1));
	this->setMaterial("green", PAGMaterial(glm::vec4(0, 1, 0, 1), glm::vec4(0, 1, 0, 1), 1));

	this->setMaterial("metal_grey", PAGMaterial(glm::vec4(0.6, 0.6, 0.6, 1), glm::vec4(0.6, 0.6, 0.6, 1), 3));
	this->setMaterial("crystal_black", PAGMaterial(glm::vec4(0.1, 0.1, 0.1, 1), glm::vec4(0.3, 0.3, 0.3, 1), 5));
}

MaterialLibrary * MaterialLibrary::getInstance()
{
	if (!instance_)
		instance_ = new MaterialLibrary();

	return instance_;
}

void MaterialLibrary::setMaterial(std::string name, const PAGMaterial& material)
{
	materials_.insert(std::pair<std::string, PAGMaterial>(name, material));
}

PAGMaterial MaterialLibrary::getMaterial(std::string name)
{
	return materials_[name];
}


MaterialLibrary::~MaterialLibrary()
{
}
