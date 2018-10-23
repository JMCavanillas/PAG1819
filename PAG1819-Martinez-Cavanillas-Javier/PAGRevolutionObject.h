#pragma once

#include <vector>
#include <GL\glew.h>
#include <glm.hpp>
#include "PAGSubdivisionProfile.h"
#include "PAGEnumeration.h"
#include "PAGStructures.h"

class PAGRevolutionObject
{
    private:
		std::vector<glm::vec3> revolution(const std::vector<glm::vec2>& points, unsigned start, unsigned end);

		std::vector<glm::vec2> calcNormals(const std::vector<glm::vec2>& points, unsigned start, unsigned end);
		std::vector<glm::vec2> calcTextureCoords(const std::vector<glm::vec2>& points, unsigned start, unsigned end);
		std::vector<glm::vec2> calcTextureCoords();
		std::vector<glm::vec3> calcTangents();

		std::vector<GLuint> calcTriangleStripIndices(unsigned points, unsigned slices);

		PAGSubdivisionProfile initialProfile_;
		PAGSubdivisionProfile refinedProfile_;

		unsigned subdivisions_;
		unsigned slices_;

		std::vector<PAGPosNorm>* posAndNorm_[3];
		std::vector<glm::vec3>*  tangents_[3];
		std::vector<glm::vec2>*  textureCoords_[3];

	public:
		PAGRevolutionObject(std::vector<glm::vec2> points, unsigned subdivisions, unsigned slices);

		bool isValid();
		bool has(PAGRevObjParts part);

		std::vector<PAGPosNorm> getPositionsAndNormals(PAGRevObjParts part);
		std::vector<glm::vec2> getTextureCoords(PAGRevObjParts part);
		std::vector<glm::vec3> getTangents(PAGRevObjParts part);

		std::vector<GLuint> getIndices4PointCloud(PAGRevObjParts part);
		std::vector<GLuint> getIndices4TriangleMesh(PAGRevObjParts part);

		unsigned getNPoints(PAGRevObjParts part);

		void exportPosNorm(PAGRevObjParts part);

		virtual ~PAGRevolutionObject();
};

