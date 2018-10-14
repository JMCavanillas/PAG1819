#include "PAGSubdivisionProfile.h"

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

	// Primer test - Vacio
	if (points.empty())
		validity.empty_test = false;

	// Segundo test - 1 punto
	if (points.size() == 1)
		validity.onePoint_test = false;

	// Tercer test - 2 puntos en X
	if (points.size() == 2 && points[0].x == 0 && points[1].x == 0)
		validity.twoPointsOnAxis_test = false;

	// Cuarto test - 2 tapas sin cuerpo
	if (points.size() == 3 && points[0].x == 0 && points[2].x == 0)
		validity.emptyBody_test = false;

	// Quinto y Sexto test - Puntos en el eje y negativos - Corrección de generatriz
	for (unsigned i = 0; i < points.size(); i++)
	{
		if (points[i].x < 0)
		{
			points[i].x = 0.001f;
			validity.negativePoints_test = false;
		}

		if (points[i].x == 0 && (i != 0 && i != points.size() - 1))
		{
			points[i].x = 0.001f;
			validity.middlePointsOnAxis_test = false;
		}	

	}

	// Decisión del estado de la generatriz
	if (validity.empty_test || validity.onePoint_test || validity.twoPointsOnAxis_test || validity.emptyBody_test)
		validity.status = PAGSubdivisionProfile::INVALID;
	else if (validity.middlePointsOnAxis_test || validity.negativePoints_test)
		validity.status = PAGSubdivisionProfile::CORRECTED;
	else
		validity.status = PAGSubdivisionProfile::GOOD;

	return validity;
}

PAGSubdivisionProfile::PAGSubdivisionProfile(std::vector<glm::vec2> points) 
	: points_(points)
{
	validityStatus_ = validation_test(this->points_);

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

		for (unsigned j = 1; i < size - 1; ++j)
		{
			resulting_polyline.push_back((subdivision_polyline[j-1] + subdivision_polyline[j]) / 2.0f); 

			resulting_polyline.push_back(
				(3.0f * subdivision_polyline[j] + ((subdivision_polyline[j - 1] + subdivision_polyline[j + 1]) / 2.0f)) / 4.0f
			);
		}

		resulting_polyline.push_back((subdivision_polyline[size-2] + subdivision_polyline[size-1]) / 2.0f);
		resulting_polyline.push_back(subdivision_polyline[size - 1]);

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


PAGSubdivisionProfile::~PAGSubdivisionProfile()
{
}
