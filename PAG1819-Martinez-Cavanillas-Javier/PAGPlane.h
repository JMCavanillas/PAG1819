#pragma once

#include <vector>

#include "PAG3DElement.h"
#include "PAGVao.h"
#include "PAGStructures.h"

class PAGPlane : public PAG3DElement
{
	private:
		unsigned hDivisions_;
		unsigned vDivisions_;
		float width_;
		float height_;

		std::vector<PAGPosNorm> posAndNorm_;
		std::vector<glm::vec3> tangents_;
		std::vector<glm::vec2> textCoords_;

		PAGVao* vao_;
	public:
		PAGPlane(float witdh = 50.0f, float height = 50.0f, unsigned hDivisions = 50, unsigned vDivisions = 50);

		void drawAsPoints(const PAGShaderProgram& shader, const glm::mat4& vp_matrix, const glm::mat4& v_matrix) override;
		void drawAsLines(const PAGShaderProgram& shader, const glm::mat4& vp_matrix, const glm::mat4& v_matrix) override;
		void drawAsTriangles(const PAGShaderProgram& shader, const glm::mat4& vp_matrix, const glm::mat4& v_matrix) override;

		virtual ~PAGPlane();
};

