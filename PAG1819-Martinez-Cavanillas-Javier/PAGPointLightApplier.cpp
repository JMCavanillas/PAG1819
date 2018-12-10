#include "PAGPointLightApplier.h"



PAGPointLightApplier::PAGPointLightApplier()
{
}

void PAGPointLightApplier::apply(const PAGShaderProgram & shader, const PAGLightSource& light)
{
	shader.setUniform("lgDiffuse", light.getDiffuse());
	shader.setUniform("lgSpecular", light.getSpecular());
	shader.setUniform("lgPosition", glm::vec3(light.getVision() * glm::vec4(light.getPosition(),1)));
}

int PAGPointLightApplier::getPreferedShader()
{
	return preferedShader_;
}


PAGPointLightApplier::~PAGPointLightApplier()
{
}
