#include "PAGAmbientLightApplier.h"



PAGAmbientLightApplier::PAGAmbientLightApplier()
{
}

void PAGAmbientLightApplier::apply(const PAGShaderProgram & shader, const PAGLightSource& light)
{
	shader.setUniform("lgAmbient", light.getAmbient());
}

int PAGAmbientLightApplier::getPreferedShader()
{
	return preferedShader_;
}


PAGAmbientLightApplier::~PAGAmbientLightApplier()
{
}
