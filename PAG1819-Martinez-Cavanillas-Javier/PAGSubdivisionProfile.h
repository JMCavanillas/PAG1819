#pragma once

#include <vector>
#include <glm.hpp>

class PAGSubdivisionProfile
{
	// Estructuras para comunicar el estado de la linea generatriz
	enum ValidityStatus
	{
		GOOD = 0,
		CORRECTED = 1,
		INVALID = 2
	};

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

	public:
		PAGSubdivisionProfile( std::vector<glm::vec2> points);
	
		PAGSubdivisionProfile subdivide(unsigned times);

		ValidityReport status();
		bool isValid();

		virtual ~PAGSubdivisionProfile();
};

