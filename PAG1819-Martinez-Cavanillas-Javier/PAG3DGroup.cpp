#include "PAG3DGroup.h"

PAG3DGroup::PAG3DGroup()
{
	modelMatrix_ = glm::mat4(1);
	material_ = PAGMaterial();
}

PAG3DGroup::PAG3DGroup(const glm::mat4& model_matrix)
{
	material_ = PAGMaterial();
	modelMatrix_ = model_matrix;
}

PAG3DGroup::PAG3DGroup(const glm::mat4 & model_matrix, const PAGMaterial & material)
{
	modelMatrix_ = model_matrix;
	material_ = material;
}

// Añade una referencia a un objeto que implemente la interfaz PAG3Delement, este objeto deberá
// ser creado en memoria dinámica y pasará a estar gestionado por PAG3DGroup (PAG3DGroup tomara la posesión del objeto)
void PAG3DGroup::add(PAG3DElement * element)
{
	elements_.push_back(element);
}

void PAG3DGroup::setMaterial(const PAGMaterial & material)
{
	for (PAG3DElement* element : elements_)
		element->setMaterial(material);
}

void PAG3DGroup::drawAsPoints(const PAGShaderProgram & shader, const glm::mat4 & vp_matrix, const glm::mat4 & v_matrix)
{
	for (PAG3DElement* element : elements_)
		element->drawAsPoints(shader, vp_matrix * modelMatrix_, v_matrix * modelMatrix_);
}

void PAG3DGroup::drawAsLines(const PAGShaderProgram & shader, const glm::mat4 & vp_matrix, const glm::mat4 & v_matrix)
{
	for (PAG3DElement* element : elements_)
		element->drawAsLines(shader, vp_matrix * modelMatrix_, v_matrix * modelMatrix_);
}

void PAG3DGroup::drawAsTriangles(const PAGShaderProgram & shader, const glm::mat4 & vp_matrix, const glm::mat4 & v_matrix)
{
	for (PAG3DElement* element : elements_)
		element->drawAsTriangles(shader, vp_matrix * modelMatrix_, v_matrix * modelMatrix_);
}


PAG3DGroup::~PAG3DGroup()
{
	for (PAG3DElement* element : elements_)
		delete element;
}
