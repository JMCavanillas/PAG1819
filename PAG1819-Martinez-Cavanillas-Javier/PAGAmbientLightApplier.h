#pragma once

#include "PAGLightApplier.h"
#include "PAGEnumeration.h"

class PAGAmbientLightApplier : public PAGLightApplier
{
	private:
		int preferedShader_ = PAG_AMBIENTLIGHT_SHADER;
	public:
		PAGAmbientLightApplier();

		void apply(const PAGShaderProgram& shader, const PAGLightSource& light) override;
		int getPreferedShader() override;

		virtual ~PAGAmbientLightApplier();
};

