#include "PAGShaderCollection.h"



PAGShaderCollection::PAGShaderCollection()
{
}

const PAGShaderProgram & PAGShaderCollection::operator[](int name)
{
	return shaders_[name];
}


void PAGShaderCollection::createShaderProgram(int name, std::string filename)
{
	PAGShaderProgram new_shader;
	shaders_.insert(std::pair<int, PAGShaderProgram>(name, new_shader));

	shaders_[name].createShaderProgram(filename.c_str());
}

PAGShaderCollection::~PAGShaderCollection()
{
}
