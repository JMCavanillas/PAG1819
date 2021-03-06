#pragma once

#include <vector>
#include "PAG3DElement.h"

class PAG3DGroup : public PAG3DElement
{
	private:
		std::vector<PAG3DElement*> elements_;
	public:
		PAG3DGroup();
		PAG3DGroup(const glm::mat4& model_matrix);
		PAG3DGroup(const glm::mat4& model_matrix, const PAGMaterial& material);

		void add(PAG3DElement* element);

		void setMaterial(const PAGMaterial& material) override;

		void drawAsPoints(const PAGShaderProgram& shader, const glm::mat4& vp_matrix, const glm::mat4& v_matrix) override;
		void drawAsLines(const PAGShaderProgram& shader, const glm::mat4& vp_matrix, const glm::mat4& v_matrix) override;
		void drawAsTriangles(const PAGShaderProgram& shader, const glm::mat4& vp_matrix, const glm::mat4& v_matrix) override;

		virtual ~PAG3DGroup();
};

