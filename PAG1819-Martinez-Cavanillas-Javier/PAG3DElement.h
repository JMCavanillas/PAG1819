#pragma once

#include <glm.hpp>
#include "PAGMaterial.h"

class PAG3DElement
{
	protected:  
		glm::mat4 modelMatrix_;
		PAGMaterial material_;

	public: 
		virtual void drawAsPoints() = 0;
		virtual void drawAsLines() = 0;
		virtual void drawAsTriangles() = 0;
};

