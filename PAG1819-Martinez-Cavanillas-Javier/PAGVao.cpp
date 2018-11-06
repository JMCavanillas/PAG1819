#include "PAGVao.h"



PAGVao::PAGVao(const std::vector<PAGPosNorm>& posNorm, const std::vector<glm::vec3>& tangents, const std::vector<glm::vec2>& textCoord)
{
	glGenVertexArrays(1, &id_);

	glBindVertexArray(id_);

	glGenBuffers(3, VBOs_);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs_[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(PAGPosNorm), ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(PAGPosNorm), ((GLubyte *)NULL + (sizeof(glm::vec3))));

	glBufferData(GL_ARRAY_BUFFER, posNorm.size() * sizeof(PAGPosNorm), posNorm.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs_[1]);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(glm::vec3), ((GLubyte *)NULL + (0)));

	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), tangents.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs_[2]);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(glm::vec2), ((GLubyte *)NULL + (0)));

	glBufferData(GL_ARRAY_BUFFER, textCoord.size() * sizeof(glm::vec2), textCoord.data(), GL_STATIC_DRAW);
}


PAGVao::~PAGVao()
{
}

void PAGVao::addIBO(const std::vector<GLuint> ibo, int name_id)
{
	if (IBOs_.find(name_id) != IBOs_.end())
		return;

	glBindVertexArray(id_);

	GLuint i_ibo;
	glGenBuffers(1, &i_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_ibo);

	IBOs_.insert(std::pair(name_id, std::pair(i_ibo, ibo.size())));

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo.size() * sizeof(GLuint), ibo.data(), GL_STATIC_DRAW);
}

bool PAGVao::draw(int ibo_name, GLenum mode)
{
	auto it = IBOs_.find(ibo_name);

	if (it == IBOs_.end())
		return false;

	glBindVertexArray(id_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->second.first);

	glDrawElements(mode, it->second.second, GL_UNSIGNED_INT, NULL);

	return true;
}



