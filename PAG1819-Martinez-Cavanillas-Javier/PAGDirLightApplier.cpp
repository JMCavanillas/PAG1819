#include "PAGDirLightApplier.h"



PAGDirLightApplier::PAGDirLightApplier()
{
}

void PAGDirLightApplier::apply(const PAGShaderProgram & shader, const PAGLightSource& light)
{
	shader.setUniform("lgDiffuse", light.getDiffuse());
	shader.setUniform("lgSpecular", light.getSpecular());
	shader.setUniform("lgDirection", glm::vec3( light.getVision() * glm::vec4(light.getDirection(), 0.0)));
}

int PAGDirLightApplier::getPreferedShader()
{
	return preferedShader_;
}


PAGDirLightApplier::~PAGDirLightApplier()
{
}
