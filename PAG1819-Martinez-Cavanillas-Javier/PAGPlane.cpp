#include "PAGPlane.h"


PAGPlane::PAGPlane(float width, float height, unsigned hDivisions, unsigned vDivisions)
{
	modelMatrix_ = glm::mat4(1);

	width_ = width;
	height_ = height;
	hDivisions_ = hDivisions;
	vDivisions_ = vDivisions;

	// Calculamos el tamaño de divisiones en horizontal y vertical
	float h_div_size = width_ / hDivisions_;
	float v_div_size = height_ / vDivisions_;

	// Vertices normales y tangentes
	glm::vec3 normal = glm::vec3(0, 1, 0);
	glm::vec3 tangent = glm::vec3(1, 0, 0);
	for (int i = 0; i <= hDivisions_; ++i)
	{
		for (int j = 0; j <= vDivisions_; ++j)
		{
			posAndNorm_.push_back({ glm::vec3(i * h_div_size, 0, j * v_div_size), normal });
			tangents_.push_back(tangent);
		}
	}

	// Coordenadas de textura
	for (int i = 0; i <= hDivisions_; ++i)
		for (int j = 0; j <= vDivisions_; ++j)
			textCoords_.push_back(glm::vec2((i * h_div_size) / width_, (j * v_div_size) / height_));

	// Creacion del VAO
	vao_ = new PAGVao(posAndNorm_, tangents_, textCoords_);

	// Topologia
	// PUNTOS
	std::vector<GLuint> point_indices;
	point_indices.reserve(posAndNorm_.size());
	for (int i = 0; i < posAndNorm_.size(); ++i)
		point_indices.push_back(i);

	vao_->addIBO(point_indices, GL_POINTS);

	// TIRA DE LINEAS
	std::vector<GLuint> lineStrip_indices;
	for (int i = 0; i <= hDivisions_; ++i)
	{
		for (int j = 0; j <= vDivisions_; ++j)
			lineStrip_indices.push_back(i * (hDivisions + 1) + j);

		lineStrip_indices.push_back(0xFFFFFFFF);
	}

	for (int j = 0; j <= vDivisions_; ++j)
	{
		for (int i = 0; i <= hDivisions_; ++i)
			lineStrip_indices.push_back(i * (hDivisions + 1) + j);

		lineStrip_indices.push_back(0xFFFFFFFF);
	}

	vao_->addIBO(lineStrip_indices, GL_LINE_STRIP);

	// TRIANGULOS
	std::vector<GLuint> triangleStrip_indices;
	for (int i = 0; i < hDivisions_; ++i)
	{
		for (int j = 0; j <= vDivisions_; ++j)
		{
			triangleStrip_indices.push_back(j * (hDivisions + 1) + i);
			triangleStrip_indices.push_back(j * (hDivisions + 1) + (i + 1));
		}
		triangleStrip_indices.push_back(0xFFFFFFFF);
	}

	vao_->addIBO(triangleStrip_indices, GL_TRIANGLE_STRIP);
}

void PAGPlane::drawAsPoints(const PAGShaderProgram & shader, const glm::mat4 & vp_matrix, const glm::mat4 & v_matrix)
{
	shader.setUniform("mModelViewProj", vp_matrix * modelMatrix_);
	vao_->draw(GL_POINTS, GL_POINTS);
}

void PAGPlane::drawAsLines(const PAGShaderProgram & shader, const glm::mat4 & vp_matrix, const glm::mat4 & v_matrix)
{
	shader.setUniform("mModelViewProj", vp_matrix * modelMatrix_);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFFFFFF);
	vao_->draw(GL_LINE_STRIP, GL_LINE_STRIP);
}

void PAGPlane::drawAsTriangles(const PAGShaderProgram & shader, const glm::mat4 & vp_matrix, const glm::mat4 & v_matrix)
{
	material_.apply(shader);
	shader.setUniform("mModelViewProj", vp_matrix * modelMatrix_);
	shader.setUniform("mModelView", v_matrix * modelMatrix_);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFFFFFF);
	vao_->draw(GL_TRIANGLE_STRIP, GL_TRIANGLE_STRIP);
}

PAGPlane::~PAGPlane()
{
	delete vao_;
}
