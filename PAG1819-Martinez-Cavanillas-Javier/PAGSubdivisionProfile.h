#pragma once

#include <vector>
#include <glm.hpp>
#include "PAGEnumeration.h"

class PAGSubdivisionProfile
{
	// Estructuras para comunicar el estado del perfil
	enum ValidityStatus
	{
		GOOD = 0,
		CORRECTED = 1,
		INVALID = 2
	};

	/**
	 * Estructura que contiene el estado del perfil y un booleano indicando si ha
	 * pasado o no cada test realizado
	 */
	struct ValidityReport
	{
		ValidityStatus status;
		bool empty_test;
		bool onePoint_test;
		bool twoPointsOnAxis_test;
		bool emptyBody_test;
		bool middlePointsOnAxis_test;
		bool negativePoints_test;
	};

	private: 
		ValidityReport validation_test(std::vector<glm::vec2>& points);

		std::vector<glm::vec2> points_;
		ValidityReport validityStatus_;

		bool hasTopLid_;
		bool hasBottomLid_;

	public:
		PAGSubdivisionProfile();
		PAGSubdivisionProfile(std::vector<glm::vec2> points);
	
		PAGSubdivisionProfile subdivide(unsigned times);

		ValidityReport status();
		bool isValid();

		bool has(PAGProfileParts part);
		const std::vector<glm::vec2>& getPoints();

		virtual ~PAGSubdivisionProfile();
};

