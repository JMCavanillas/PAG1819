#include "PAG3DElement.h"

void PAG3DElement::setModelMatrix(const glm::mat4 & model_matrix)
{
	modelMatrix_ = model_matrix;
}

void PAG3DElement::setMaterial(const PAGMaterial & material)
{
	material_ = material;
}

glm::mat4 PAG3DElement::getModelMatrix()
{
	return modelMatrix_;
}

PAGMaterial PAG3DElement::getMaterial()
{
	return material_;
}
