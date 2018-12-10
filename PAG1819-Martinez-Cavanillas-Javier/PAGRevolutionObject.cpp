#include "PAGRevolutionObject.h"

#include <cmath>
#include <fstream>

const double PI = 3.14159265358979323846;

/**
 * Constructor de la clase, genera el objeto de revolución indicado
 */
PAGRevolutionObject::PAGRevolutionObject(std::vector<glm::vec2> points, unsigned subdivisions, unsigned slices)
{
	modelMatrix_ = glm::mat4(1);
	material_ = PAGMaterial();

	initialProfile_ = PAGSubdivisionProfile(points);
	refinedProfile_ = initialProfile_.subdivide(subdivisions);

	subdivisions_ = subdivisions;
	slices_ = slices;

	for (unsigned i = 0; i < 3; ++i)
	{
		VAOs_[i] = nullptr;
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

	VAOs_[PAGRevObjParts::PAG_BODY] = new PAGVao(*posAndNorm_[PAGRevObjParts::PAG_BODY],
		*tangents_[PAGRevObjParts::PAG_BODY], *textureCoords_[PAGRevObjParts::PAG_BODY]);
	
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

		VAOs_[PAGRevObjParts::PAG_BOTTOM_FAN] = new PAGVao(*posAndNorm_[PAGRevObjParts::PAG_BOTTOM_FAN],
			*tangents_[PAGRevObjParts::PAG_BOTTOM_FAN], *textureCoords_[PAGRevObjParts::PAG_BOTTOM_FAN]);
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

		VAOs_[PAGRevObjParts::PAG_TOP_FAN] = new PAGVao(*posAndNorm_[PAGRevObjParts::PAG_TOP_FAN],
			*tangents_[PAGRevObjParts::PAG_TOP_FAN], *textureCoords_[PAGRevObjParts::PAG_TOP_FAN]);
	}

	// Añadimos la topologia
	// PUNTOS
	for (unsigned p = 0; p < 3; ++p) 
		if (VAOs_[p])
		{
			std::vector<GLuint> indices;
			indices.reserve(posAndNorm_[p]->size());
			for (unsigned i = 0; i < posAndNorm_[p]->size(); ++i)
				indices.push_back(i);

			VAOs_[p]->addIBO(indices, GL_POINTS);
		}

	// TIRA DE LINEAS
	for (unsigned p = 0; p < 3; ++p)
		if (VAOs_[p])
		{
			if (p == PAGRevObjParts::PAG_BODY)
			{
				std::vector<GLuint> indices = calcLineStripIndices(posAndNorm_[PAGRevObjParts::PAG_BODY]->size() / (slices_ + 1), slices_);
				VAOs_[p]->addIBO(indices, GL_LINE_STRIP);
			} 
			else {
				std::vector<GLuint> indices;
				for (unsigned i = 2; i < posAndNorm_[p]->size(); ++i)
				{
					indices.push_back(0);
					indices.push_back(i);
					indices.push_back(i - 1);
					indices.push_back(0xFFFFFFFF);
				}

				VAOs_[p]->addIBO(indices, GL_LINE_STRIP);
			}
		}

	// TIRA DE TRIANGULOS
	for (unsigned p = 0; p < 3; ++p)
		if (VAOs_[p])
		{
			if (p == PAGRevObjParts::PAG_BODY)
			{
				std::vector<GLuint> indices = calcTriangleStripIndices(posAndNorm_[PAGRevObjParts::PAG_BODY]->size() / (slices_ + 1), slices_);
				VAOs_[p]->addIBO(indices, GL_TRIANGLE_STRIP);
			}
			else {
				std::vector<GLuint> indices;
				for (unsigned i = 0; i < posAndNorm_[p]->size(); ++i)
					indices.push_back(i);

				VAOs_[p]->addIBO(indices, GL_TRIANGLE_FAN);
			}
		}
}

PAGRevolutionObject::PAGRevolutionObject(std::vector<glm::vec2> points, unsigned subdivisions, unsigned slices, const glm::mat4 & model_matrix)
	: PAGRevolutionObject(points, subdivisions, slices)
{
	modelMatrix_ = model_matrix;
}

PAGRevolutionObject::PAGRevolutionObject(std::vector<glm::vec2> points, unsigned subdivisions, unsigned slices, const glm::mat4 & model_matrix, const PAGMaterial & material)
	: PAGRevolutionObject(points, subdivisions, slices, model_matrix)
{
	material_ = material;
}

PAGRevolutionObject::PAGRevolutionObject(const PAGRevolutionObject & orig)
{
	initialProfile_ = orig.initialProfile_;
	refinedProfile_ = orig.refinedProfile_;
	subdivisions_ = orig.subdivisions_;
	slices_ = orig.slices_;

	material_ = orig.material_;
	modelMatrix_ = orig.modelMatrix_;

	for (unsigned i = 0; i < 3; ++i)
	{
		VAOs_[i] = nullptr;
		posAndNorm_[i] = nullptr;
		tangents_[i] = nullptr;
		textureCoords_[i] = nullptr;
		if (orig.VAOs_[i])
			VAOs_[i] = new PAGVao(*orig.VAOs_[i]);
		if (orig.posAndNorm_[i])
			posAndNorm_[i] = new std::vector<PAGPosNorm>(*orig.posAndNorm_[i]);
		if (orig.tangents_[i])
			tangents_[i] = new std::vector<glm::vec3>(*orig.tangents_[i]);
		if (orig.textureCoords_[i])
			textureCoords_[i] = new std::vector<glm::vec2>(*orig.textureCoords_[i]);

	}
}

/**
 * Destructor de la clase, libera la memoria alojada
 */
PAGRevolutionObject::~PAGRevolutionObject()
{
	for (unsigned i = 0; i < 3; ++i)
	{
		if (VAOs_[i])
			delete VAOs_[i];
		if (posAndNorm_[i])
			delete posAndNorm_[i];
		if (tangents_[i])
			delete tangents_[i];
		if(textureCoords_[i])
			delete textureCoords_[i];
	}
}

/**
 * Exporta puntos y normales a un archivo de texto (Por motivos de depuración)
 */
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

void PAGRevolutionObject::drawAsPoints(const PAGShaderProgram& shader, const glm::mat4& vp_matrix, const glm::mat4& v_matrix)
{
	shader.setUniform("mModelViewProj",  vp_matrix * modelMatrix_);
	for (unsigned p = 0; p < 3; ++p)
		if (VAOs_[p])
			VAOs_[p]->draw(GL_POINTS, GL_POINTS);
}

void PAGRevolutionObject::drawAsLines(const PAGShaderProgram& shader, const glm::mat4& vp_matrix, const glm::mat4& v_matrix)
{
	shader.setUniform("mModelViewProj", vp_matrix * modelMatrix_);
	for (unsigned p = 0; p < 3; ++p)
		if (VAOs_[p])
		{
			glEnable(GL_PRIMITIVE_RESTART);
			glPrimitiveRestartIndex(0xFFFFFFFF);
			VAOs_[p]->draw(GL_LINE_STRIP, GL_LINE_STRIP);
		}
}

void PAGRevolutionObject::drawAsTriangles(const PAGShaderProgram& shader, const glm::mat4& vp_matrix, const glm::mat4& v_matrix)
{
	material_.apply(shader);
	shader.setUniform("mModelViewProj", vp_matrix * modelMatrix_);
	shader.setUniform("mModelView", v_matrix * modelMatrix_);
	for (unsigned p = 0; p < 3; ++p)
		if (VAOs_[p])
		{
			glEnable(GL_PRIMITIVE_RESTART);
			glPrimitiveRestartIndex(0xFFFFFFFF);

			if (p == PAGRevObjParts::PAG_BODY)
				VAOs_[p]->draw(GL_TRIANGLE_STRIP, GL_TRIANGLE_STRIP);
			else 
				VAOs_[p]->draw(GL_TRIANGLE_FAN, GL_TRIANGLE_FAN);
		}
}

/**
 * Devuelve un conjunto con los puntos (o normales) indicados revolucionados de acuerdo al atributo slices_
 */
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
			glm::vec3 r_point (points[i].x * cos(alpha), points[i].y, -1 * points[i].x * sin(alpha)); 

			result_points.push_back(r_point);
		}
	}

	return result_points;
}

/**
 * Devuelve las tangentes de un nivel del puntos
 */
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


/**
 * Calcula los indices indicando la topología para el cuerpo del objeto de revolución
 * points = Numero de puntos del cuerpo
 */
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
		indices.push_back(0xFFFFFFFF);
	}
	return indices;
}

std::vector<GLuint> PAGRevolutionObject::calcLineStripIndices(unsigned points, unsigned slices)
{
	std::vector<GLuint> indices;
	for (unsigned i = 0; i < points; ++i)
	{
		for (unsigned s = 0; s <= slices; ++s)
			indices.push_back(i*(slices + 1) + s);
		indices.push_back(0xFFFFFFFF);
	}

	for (unsigned s = 0; s < slices; ++s)
	{
		for (unsigned i = 0; i < points; ++i)
			indices.push_back(i*(slices + 1) + s);

		indices.push_back(0xFFFFFFFF);
	}
	return indices;
}

/**
 * Calcula las normales de una linea generatriz
 */
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

/**
 * Calcula las coordenadas de textura para el cuerpo de un objeto de revolución
 */
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
			textureCoords.push_back(glm::vec2(float(j) / slices_, lengths[i]/lengths.back() ));

	return textureCoords;
}

/**
 * Calcula las coordenadas de textura para las tapas de un objeto de revolución
 */
std::vector<glm::vec2> PAGRevolutionObject::calcTextureCoords()
{
	double delta = 2 * PI / slices_;

	std::vector<glm::vec2> textureCoords;
	textureCoords.reserve(slices_ + 1);

	// Primer Elemento de la tapa
	textureCoords.push_back(glm::vec2(0.5, 0.5));

	for (unsigned s = 0; s <= slices_; ++s)
		textureCoords.push_back(glm::vec2((cos(s*delta) / 2.0f + 0.5f), (sin(s*delta) / 2.0f) + 0.5f));

	return textureCoords;
}

/**
 * Devuelve si un objeto tiene un perfil válido
 */
bool PAGRevolutionObject::isValid()
{
	return initialProfile_.isValid();
}

/**
 * Devuelve si un objeto tiene la parte especificada
 */
bool PAGRevolutionObject::has(PAGRevObjParts part)
{
	return posAndNorm_[part];
}

/**
 * Devuelve el numero de puntos totales de la parte especificada
 */
unsigned PAGRevolutionObject::getNPoints(PAGRevObjParts part)
{
	if (posAndNorm_[part])
		posAndNorm_[part]->size();

	return 0;
}
