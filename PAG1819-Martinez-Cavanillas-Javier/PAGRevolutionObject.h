#pragma once

#include <vector>
#include <GL\glew.h>
#include <glm.hpp>
#include "PAG3DElement.h"
#include "PAGSubdivisionProfile.h"
#include "PAGEnumeration.h"
#include "PAGStructures.h"
#include "PAGVao.h"

class PAGRevolutionObject : public PAG3DElement
{
    private:
		std::vector<glm::vec3> revolution(const std::vector<glm::vec2>& points, unsigned start, unsigned end);

		std::vector<glm::vec2> calcNormals(const std::vector<glm::vec2>& points, unsigned start, unsigned end);
		std::vector<glm::vec2> calcTextureCoords(const std::vector<glm::vec2>& points, unsigned start, unsigned end);
		std::vector<glm::vec2> calcTextureCoords();
		std::vector<glm::vec3> calcTangents();

		std::vector<GLuint> calcTriangleStripIndices(unsigned points, unsigned slices);
		std::vector<GLuint> calcLineStripIndices(unsigned points, unsigned slices);

		PAGSubdivisionProfile initialProfile_;
		PAGSubdivisionProfile refinedProfile_;

		unsigned subdivisions_;
		unsigned slices_;

		std::vector<PAGPosNorm>* posAndNorm_[3];
		std::vector<glm::vec3>*  tangents_[3];
		std::vector<glm::vec2>*  textureCoords_[3];

		PAGVao* VAOs_[3];

	public:
		PAGRevolutionObject(std::vector<glm::vec2> points, unsigned subdivisions, unsigned slices);
		PAGRevolutionObject(std::vector<glm::vec2> points, unsigned subdivisions, unsigned slices, const glm::mat4& model_matrix);
		PAGRevolutionObject(std::vector<glm::vec2> points, unsigned subdivisions, unsigned slices, const glm::mat4& model_matrix, 
			const PAGMaterial& material);
		PAGRevolutionObject(const PAGRevolutionObject& orig);

		bool isValid();
		bool has(PAGRevObjParts part);

		unsigned getNPoints(PAGRevObjParts part);

		void exportPosNorm(PAGRevObjParts part);

		void drawAsPoints(const PAGShaderProgram& shader, const glm::mat4& vp_matrix, const glm::mat4& v_matrix);
		void drawAsLines(const PAGShaderProgram& shader, const glm::mat4& vp_matrix, const glm::mat4& v_matrix);
		void drawAsTriangles(const PAGShaderProgram& shader, const glm::mat4& vp_matrix, const glm::mat4& v_matrix);

		virtual ~PAGRevolutionObject();
};

