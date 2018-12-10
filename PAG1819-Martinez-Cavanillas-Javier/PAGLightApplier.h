#pragma once

class PAGLightSource;
#include "PAGLightSource.h"
#include "PAGShaderProgram.h"

class PAGLightApplier
{
	public:
		virtual void apply(const PAGShaderProgram& shader, const PAGLightSource& light) = 0;
		virtual int getPreferedShader() = 0;
};

