#pragma once

#include <map>
#include <string>
#include "PAGShaderProgram.h"

class PAGShaderCollection
{
	private:
		
	public:
		std::map<int, PAGShaderProgram> shaders_;

		PAGShaderCollection();

		const PAGShaderProgram& operator[](int name);

		void createShaderProgram(int name, std::string filename);
		virtual ~PAGShaderCollection();
};

