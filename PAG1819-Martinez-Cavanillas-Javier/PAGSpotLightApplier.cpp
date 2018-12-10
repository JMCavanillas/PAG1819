#include "PAGSpotLightApplier.h"



PAGSpotLightApplier::PAGSpotLightApplier()
{
}

void PAGSpotLightApplier::apply(const PAGShaderProgram & shader, const PAGLightSource& light)
{
	shader.setUniform("lgDiffuse", light.getDiffuse());
	shader.setUniform("lgSpecular", light.getSpecular());
	shader.setUniform("lgDirection", glm::vec3(light.getVision() * glm::vec4(light.getDirection(), 0)));
	shader.setUniform("lgPosition", glm::vec3(light.getVision() * glm::vec4(light.getPosition(), 1)));
	shader.setUniform("lgScattAng", light.getScatterAngle());
	shader.setUniform("lgScattExp", light.getScatterExp());
}

int PAGSpotLightApplier::getPreferedShader()
{
	return preferedShader_;
}


PAGSpotLightApplier::~PAGSpotLightApplier()
{
}
