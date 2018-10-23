#include "PAGSubdivisionProfile.h"

#include <gtc/constants.hpp>

PAGSubdivisionProfile::ValidityReport PAGSubdivisionProfile::validation_test(std::vector<glm::vec2>& points)
{
	ValidityReport validity;

	validity.empty_test = 
		validity.onePoint_test = 
		validity.twoPointsOnAxis_test = 
		validity.emptyBody_test =
		validity.middlePointsOnAxis_test = 
		validity.negativePoints_test =
		true;

	// Test - Puntos en el eje y negativos - Corrección de generatriz
	for (auto it = points.begin(); it != points.end(); ++it)
	{
		if (it->x < 0 - glm::epsilon<float>())
		{
			it = points.erase(it);
			validity.negativePoints_test = false;
		}
	}

	for (auto it = points.begin() + 1; it != points.end() - 1; ++it)
	{
		unsigned i = std::distance(points.begin(), it);
		if ((points[i].x > 0 - glm::epsilon<float>() 
			&& points[i].x < 0 + glm::epsilon<float>()))
		{
			it = points.erase(it);
			validity.middlePointsOnAxis_test = false;
		}	
	}

	// Test - Vacio
	if (points.empty())
		validity.empty_test = false;

	// Test - 1 punto
	if (points.size() == 1)
		validity.onePoint_test = false;

	// Test - 2 puntos en X
	if (points.size() == 2
		&& (points[0].x > 0 - glm::epsilon<float>() && points[0].x < 0 + glm::epsilon<float>())
		&& (points[1].x > 0 - glm::epsilon<float>() && points[1].x < 0 + glm::epsilon<float>()))
		validity.twoPointsOnAxis_test = false;

	// Test - 2 tapas sin cuerpo
	if (points.size() == 3
		&& (points[0].x > 0 - glm::epsilon<float>() && points[0].x < 0 + glm::epsilon<float>())
		&& (points[2].x > 0 - glm::epsilon<float>() && points[2].x < 0 + glm::epsilon<float>()))
		validity.emptyBody_test = false;

	// Decisión del estado de la generatriz
	if (!(validity.empty_test || validity.onePoint_test || validity.twoPointsOnAxis_test || validity.emptyBody_test))
		validity.status = PAGSubdivisionProfile::INVALID;
	else if (!(validity.middlePointsOnAxis_test || validity.negativePoints_test))
		validity.status = PAGSubdivisionProfile::CORRECTED;
	else
		validity.status = PAGSubdivisionProfile::GOOD;

	return validity;
}

PAGSubdivisionProfile::PAGSubdivisionProfile()
{

}

PAGSubdivisionProfile::PAGSubdivisionProfile(std::vector<glm::vec2> points)
	: points_(points)
{
	validityStatus_ = validation_test(this->points_);

	hasBottomLid_ = (points[0].x > 0 - glm::epsilon<float>() && points[0].x < 0 + glm::epsilon<float>());
	hasTopLid_ = (points[points_.size() - 1].x > 0 - glm::epsilon<float>() && points[points_.size() - 1].x < 0 + glm::epsilon<float>());

}

PAGSubdivisionProfile PAGSubdivisionProfile::subdivide(unsigned times)
{
	if (validityStatus_.status == PAGSubdivisionProfile::INVALID)
		throw new std::invalid_argument("The profile is invalid, cannot subdivide," 
			"check PAGSubdivisionProfile::status() for further Information");

	std::vector<glm::vec2> subdivision_polyline = points_;

	for (unsigned i = 0; i < times; ++i)
	{
		std::vector<glm::vec2> resulting_polyline;

		unsigned size = subdivision_polyline.size();

		resulting_polyline.push_back(subdivision_polyline[0]);

		for (unsigned j = 1; j < size - 1; ++j)
		{
			resulting_polyline.push_back((subdivision_polyline[j-1] + subdivision_polyline[j]) / 2.0f); 

			resulting_polyline.push_back(
				(3.0f * subdivision_polyline[j] + ((subdivision_polyline[j - 1] + subdivision_polyline[j + 1]) / 2.0f)) / 4.0f
			);
		}

		resulting_polyline.push_back((subdivision_polyline[size-2] + subdivision_polyline[size-1]) / 2.0f);
		resulting_polyline.push_back(subdivision_polyline[size-1]);

		subdivision_polyline = std::move(resulting_polyline);
	}

	return PAGSubdivisionProfile(subdivision_polyline);
}

PAGSubdivisionProfile::ValidityReport PAGSubdivisionProfile::status()
{
	return this->validityStatus_;
}

bool PAGSubdivisionProfile::isValid()
{
	return !(validityStatus_.status == PAGSubdivisionProfile::INVALID);
}

bool PAGSubdivisionProfile::has(PAGProfileParts part)
{
	if (validityStatus_.status == PAGSubdivisionProfile::INVALID)
		throw new std::invalid_argument("The profile is invalid, cannot subdivide,"
			"check PAGSubdivisionProfile::status() for further Information");

	if (part == PAGProfileParts::PAG_TOP_LID)
		return hasTopLid_;
	if (part == PAGProfileParts::PAG_BOTTOM_LID)
		return hasBottomLid_;
	if (part == PAGProfileParts::PAG_PROF_BODY)
		return true;
}

const std::vector<glm::vec2>& PAGSubdivisionProfile::getPoints()
{
	return points_;
}


PAGSubdivisionProfile::~PAGSubdivisionProfile()
{
}
