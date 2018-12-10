#pragma once

#include "PAGLightApplier.h"
#include "PAGEnumeration.h"

class PAGDirLightApplier : public PAGLightApplier
{
	private:
		int preferedShader_ = PAG_DIRLIGHT_SHADER;
	public:
		PAGDirLightApplier();

		void apply(const PAGShaderProgram& shader, const PAGLightSource& light) override;
		int getPreferedShader() override;

		virtual ~PAGDirLightApplier();
};

