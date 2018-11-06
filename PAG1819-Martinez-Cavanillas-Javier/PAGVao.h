#pragma once
#include <GL/glew.h>
#include <map>
#include <vector>
#include "PAGStructures.h"

class PAGVao
{
	private: 
		GLuint id_;

		GLuint VBOs_[3];

		std::map<int, std::pair<GLuint, GLsizei>> IBOs_;

	public:
		PAGVao(const std::vector<PAGPosNorm>& posNorm, const std::vector<glm::vec3>& tangents, const std::vector<glm::vec2>& textCoord);
		~PAGVao();

		void addIBO(const std::vector<GLuint> ibo, int name_id);

		bool draw(int ibo_name, GLenum mode);

};

