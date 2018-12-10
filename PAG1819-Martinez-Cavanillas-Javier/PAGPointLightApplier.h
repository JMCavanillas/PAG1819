#pragma once

#include "PAGLightApplier.h"
#include "PAGEnumeration.h"

class PAGPointLightApplier : public PAGLightApplier
{
	private:
		int preferedShader_ = PAG_POINTLIGHT_SHADER;
	public:
		PAGPointLightApplier();

		void apply(const PAGShaderProgram& shader, const PAGLightSource& light) override;
		int getPreferedShader() override;

		virtual ~PAGPointLightApplier();
};

