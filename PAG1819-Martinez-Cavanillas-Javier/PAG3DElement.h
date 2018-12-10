#pragma once

#include <glm.hpp>
#include "PAGMaterial.h"
#include "PAGShaderProgram.h"

class PAG3DElement
{
	protected:  
		glm::mat4 modelMatrix_;
		PAGMaterial material_;

	public: 

		virtual void setModelMatrix(const glm::mat4& model_matrix);
		virtual void setMaterial(const PAGMaterial& material);

		virtual glm::mat4 getModelMatrix();
		virtual PAGMaterial getMaterial();

		virtual void drawAsPoints(const PAGShaderProgram& shader, const glm::mat4& vp_matrix, const glm::mat4& v_matrix) = 0;
		virtual void drawAsLines(const PAGShaderProgram& shader, const glm::mat4& vp_matrix, const glm::mat4& v_matrix) = 0;
		virtual void drawAsTriangles(const PAGShaderProgram& shader, const glm::mat4& vp_matrix, const glm::mat4& v_matrix) = 0;
};

