#include "PAGRevolutionObject.h"

#include <cmath>
#include <fstream>

const double PI = 3.14159265358979323846;

PAGRevolutionObject::PAGRevolutionObject(std::vector<glm::vec2> points, unsigned subdivisions, unsigned slices)
{
	initialProfile_ = PAGSubdivisionProfile(points);
	refinedProfile_ = initialProfile_.subdivide(subdivisions);

	subdivisions_ = subdivisions;
	slices_ = slices;

	for (unsigned i = 0; i < 3; ++i)
	{
		posAndNorm_[i] = nullptr;
		tangents_[i] = nullptr;
		textureCoords_[i] = nullptr;
	}

	// Preguntamos si hay tapas
	bool top_lid = refinedProfile_.has(PAGProfileParts::PAG_TOP_LID);
	bool bottom_lid = refinedProfile_.has(PAGProfileParts::PAG_BOTTOM_LID);

	// Obtenemos el cuerpo
	unsigned start = 0;
	unsigned end = refinedProfile_.getPoints().size();
	
	if (bottom_lid)
		start = 1;
	if (top_lid)
		end = refinedProfile_.getPoints().size() - 1;

	// Calcular Normales del perfil
	std::vector<glm::vec2> profile_normals = calcNormals(refinedProfile_.getPoints(), start, end);

	// Revolucionar normales y puntos
	std::vector<glm::vec3> body_points = revolution(refinedProfile_.getPoints(), start, end);
	std::vector<glm::vec3> body_normals = revolution(profile_normals, 0, profile_normals.size());

	// Calcular las tangentes de una fila
	std::vector<glm::vec3> body_tangents = calcTangents();
	
	// Empaquetar valores
	posAndNorm_[PAGRevObjParts::PAG_BODY] = new std::vector<PAGPosNorm>();
	tangents_[PAGRevObjParts::PAG_BODY] = new std::vector<glm::vec3>();
	textureCoords_[PAGRevObjParts::PAG_BODY] = new std::vector<glm::vec2>();
	*textureCoords_[PAGRevObjParts::PAG_BODY] = calcTextureCoords(refinedProfile_.getPoints(), start, end);

	for (unsigned i = 0; i < body_points.size(); ++i)
	{
		PAGPosNorm element = { body_points[i], body_normals[i] };
		posAndNorm_[PAGRevObjParts::PAG_BODY]->push_back(element);
		tangents_[PAGRevObjParts::PAG_BODY]->push_back(body_tangents[i % body_tangents.size()]);
	}
	
	// Tratamiento de tapas
	if (bottom_lid)
	{
		posAndNorm_[PAGRevObjParts::PAG_BOTTOM_FAN] = new std::vector<PAGPosNorm>();
		tangents_[PAGRevObjParts::PAG_BOTTOM_FAN] = new std::vector<glm::vec3>();
		textureCoords_[PAGRevObjParts::PAG_BOTTOM_FAN] = new std::vector<glm::vec2>();
		*textureCoords_[PAGRevObjParts::PAG_BOTTOM_FAN] = calcTextureCoords();

		PAGPosNorm first_vertex = { glm::vec3(0, refinedProfile_.getPoints()[0].y, 0), glm::vec3(0, -1, 0) };
		posAndNorm_[PAGRevObjParts::PAG_BOTTOM_FAN]->push_back(first_vertex);

		glm::vec3 z_positive = glm::vec3(0, 0, 1);
		tangents_[PAGRevObjParts::PAG_BOTTOM_FAN]->push_back(glm::cross(first_vertex.normal, z_positive));

		// Al ser unsigned desborda en el -1 y se vuelve mayor que slices, cortando el bucle
		for (unsigned i = slices_; i <= slices_; --i)
		{
			PAGPosNorm vertex = (*posAndNorm_[PAGRevObjParts::PAG_BODY])[i];
			posAndNorm_[PAGRevObjParts::PAG_BOTTOM_FAN]->push_back(vertex);
			tangents_[PAGRevObjParts::PAG_BOTTOM_FAN]->push_back(glm::cross(vertex.normal, z_positive));
		}
	}

	if (top_lid)
	{
		posAndNorm_[PAGRevObjParts::PAG_TOP_FAN] = new std::vector<PAGPosNorm>();
		tangents_[PAGRevObjParts::PAG_TOP_FAN] = new std::vector<glm::vec3>();
		textureCoords_[PAGRevObjParts::PAG_TOP_FAN] = new std::vector<glm::vec2>();
		*textureCoords_[PAGRevObjParts::PAG_TOP_FAN] = calcTextureCoords();

		PAGPosNorm first_vertex = { glm::vec3(0, 
									refinedProfile_.getPoints()[refinedProfile_.getPoints().size()-1].y, 0),
									glm::vec3(0, 1, 0) };
		posAndNorm_[PAGRevObjParts::PAG_TOP_FAN]->push_back(first_vertex);

		glm::vec3 z_positive = glm::vec3(0, 0, 1);
		tangents_[PAGRevObjParts::PAG_TOP_FAN]->push_back(glm::cross(first_vertex.normal, z_positive));

		for (unsigned i = 0; i <= slices_; ++i)
		{
			unsigned v_start = posAndNorm_[PAGRevObjParts::PAG_BODY]->size() - slices_ - 1;
			PAGPosNorm vertex = (*posAndNorm_[PAGRevObjParts::PAG_BODY])[v_start + i];
			posAndNorm_[PAGRevObjParts::PAG_TOP_FAN]->push_back(vertex);
			tangents_[PAGRevObjParts::PAG_TOP_FAN]->push_back(glm::cross(vertex.normal, z_positive));
		}
	}

}

PAGRevolutionObject::~PAGRevolutionObject()
{
	for (unsigned i = 0; i < 3; ++i)
	{
		if (posAndNorm_[i])
			delete posAndNorm_[i];
		if (tangents_[i])
			delete tangents_[i];
		if(textureCoords_[i])
			delete textureCoords_[i];
	}
}

void PAGRevolutionObject::exportPosNorm(PAGRevObjParts part)
{
	std::ofstream file("myfile.txt");

	if (posAndNorm_[part])
	{
		for (PAGPosNorm vert : *posAndNorm_[part])
		{
			file << vert.position.x << " " << vert.position.y << " " << vert.position.z << " "
				<< vert.normal.x << " " << vert.normal.y << " " << vert.normal.z << std::endl;
		}

		//for (unsigned i = 0; i < posAndNorm_[part]->size(); ++i)
		//{
		//	PAGPosNorm vert = (*posAndNorm_[part])[i];
		//	glm::vec3 tg = (*tangents_[part])[i];

		//	file << vert.position.x << " " << vert.position.y << " " << vert.position.z << " "
		//		<< tg.x << " " << tg.y << " " << tg.z << std::endl;

		//}
	}
		
}

std::vector<glm::vec3> PAGRevolutionObject::revolution(const std::vector<glm::vec2>& points, unsigned start, unsigned end)
{
	

	if (end > points.size() || start > points.size())
		throw new std::invalid_argument("Out of bounds start or end indexes");

	double delta = (2 * PI) / slices_;

	std::vector<glm::vec3> result_points;

	for (unsigned i = start; i < end; ++i)
	{
		for (unsigned s = 0; s <= slices_; ++s)
		{
			float alpha = s * delta;
			glm::vec3 r_point (points[i].x * cos(s*delta), points[i].y, -1 * points[i].x * sin(s*delta)); 

			result_points.push_back(r_point);
		}
	}

	return result_points;
}

std::vector<glm::vec3> PAGRevolutionObject::calcTangents()
{
	double delta = 2 * PI / slices_;

	std::vector<glm::vec3> result_points;

	for (unsigned s = 0; s <= slices_; ++s)
	{
		float alpha = s * delta;
		glm::vec3 r_point(-1 * sin(s*delta), 0, -1 * cos(s*delta));

		result_points.push_back(r_point);
	}

	return result_points;
}

std::vector<GLuint> PAGRevolutionObject::calcTriangleStripIndices(unsigned points, unsigned slices)
{
	std::vector<GLuint> indices;
	for (unsigned s = 0; s < slices; ++s)
	{
		for (unsigned i = 0; i < points; ++i)
		{
			indices.push_back(i*(slices + 1) + s);
			indices.push_back(i*(slices + 1) + (s + 1));
		}
		indices.push_back(0xFFFF);
	}
	return indices;
}

std::vector<glm::vec2> PAGRevolutionObject::calcNormals(const std::vector<glm::vec2>& points, unsigned start, unsigned end)
{
	if (end > points.size() || start > points.size())
		throw new std::invalid_argument("Out of bounds start or end indexes");

	std::vector<glm::vec2> result;
	glm::vec2 first_edge = points[start+1] - points[start];

	glm::vec2 prev_normal = glm::vec2(first_edge.y, -1 * first_edge.x);
	prev_normal = glm::normalize(prev_normal);
	result.push_back(prev_normal);

	glm::vec2 next_normal;

	for (unsigned i = start + 1; i < end - 1; ++i)
	{
		glm::vec2 edge = points[i+1] - points[i];
		next_normal = glm::vec2(edge.y, -1 * edge.x);
		next_normal = normalize(next_normal);

		glm::vec2 vertex_normal = (prev_normal + next_normal) / 2.0f;

		vertex_normal = glm::normalize(vertex_normal);

		result.push_back(vertex_normal);

		prev_normal = next_normal;
	}

	result.push_back(next_normal);
	return result;
}

std::vector<glm::vec2> PAGRevolutionObject::calcTextureCoords(const std::vector<glm::vec2>& points, unsigned start, unsigned end)
{
	if (end > points.size() || start > points.size())
		throw new std::invalid_argument("Out of bounds start or end indexes");
	
	std::vector<float> lengths;
	lengths.reserve(end - start);
	// la distancia recorrida en el punto 0 es 0
	lengths.push_back(0);
	for (unsigned i = start + 1; i < end; ++i)
	{
		float length = glm::length(points[i] - points[i-1]) + lengths.back();
		lengths.push_back(length);
	}

	std::vector<glm::vec2> textureCoords;
	for (unsigned i = 0; i < end - start; ++i)
		for (unsigned j = 0; j <= slices_; ++j)
			textureCoords.push_back(glm::vec2(j / slices_, lengths[i]/lengths.back() ));

	return textureCoords;
}

std::vector<glm::vec2> PAGRevolutionObject::calcTextureCoords()
{
	double delta = 2 * PI / slices_;

	std::vector<glm::vec2> textureCoords;
	textureCoords.reserve(slices_ + 1);

	// Primer Elemento de la tapa
	textureCoords.push_back(glm::vec2(0.5, 0.5));

	for (unsigned s = 0; s < slices_; ++s)
		textureCoords.push_back(glm::vec2((cos(s*delta) / 2.0f + 0.5f), (sin(s*delta) / 2.0f) + 0.5f));

	return textureCoords;
}

bool PAGRevolutionObject::isValid()
{
	return initialProfile_.isValid();
}

bool PAGRevolutionObject::has(PAGRevObjParts part)
{
	return posAndNorm_[part];
}

std::vector<PAGPosNorm> PAGRevolutionObject::getPositionsAndNormals(PAGRevObjParts part)
{
	if (posAndNorm_[part])
		return *posAndNorm_[part];

	return std::vector<PAGPosNorm>();
}

std::vector<glm::vec2> PAGRevolutionObject::getTextureCoords(PAGRevObjParts part)
{
	if (textureCoords_[part])
		return *textureCoords_[part];

	return std::vector<glm::vec2>();
}

std::vector<glm::vec3> PAGRevolutionObject::getTangents(PAGRevObjParts part)
{
	if (tangents_[part])
		return *tangents_[part];

	return std::vector<glm::vec3>();
}

std::vector<GLuint> PAGRevolutionObject::getIndices4PointCloud(PAGRevObjParts part)
{
	if (posAndNorm_[part])
	{
		std::vector<GLuint> indices;
		indices.reserve(posAndNorm_[part]->size());
		for (unsigned i = 0; i < posAndNorm_[part]->size(); ++i)
			indices.push_back(i);
		return indices;
	}

	return std::vector<GLuint>();
}

std::vector<GLuint> PAGRevolutionObject::getIndices4TriangleMesh(PAGRevObjParts part)
{
	if (!posAndNorm_[part])
		return std::vector<GLuint>();

	std::vector<GLuint> indices;
	indices.reserve(posAndNorm_[part]->size());

	if (part == PAGRevObjParts::PAG_BOTTOM_FAN || part == PAGRevObjParts::PAG_TOP_FAN)
	{	
		for (unsigned i = 0; i < posAndNorm_[part]->size(); ++i)
			indices.push_back(i);
		return indices;
	}

	if (part == PAGRevObjParts::PAG_BODY)
		return calcTriangleStripIndices(posAndNorm_[part]->size() / (slices_ + 1), slices_);
}

unsigned PAGRevolutionObject::getNPoints(PAGRevObjParts part)
{
	if (posAndNorm_[part])
		posAndNorm_[part]->size();

	return 0;
}
