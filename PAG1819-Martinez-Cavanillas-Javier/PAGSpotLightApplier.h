#pragma once

#include "PAGLightApplier.h"
#include "PAGEnumeration.h"

class PAGSpotLightApplier : public PAGLightApplier
{
	private:
		int preferedShader_ = PAG_SPOTLIGHT_SHADER;
	public:
		PAGSpotLightApplier();

		void apply(const PAGShaderProgram& shader, const PAGLightSource& light) override;
		int getPreferedShader() override;

		virtual ~PAGSpotLightApplier();
};

