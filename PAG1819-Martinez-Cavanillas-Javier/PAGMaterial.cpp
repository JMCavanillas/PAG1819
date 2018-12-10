#include "PAGMaterial.h"

PAGMaterial::PAGMaterial()
{
	Kd_ = glm::vec4(0.4, 0.2, 0.3, 1);
	Ks_ = glm::vec4(0.3);
	shininess_ = 2;
}

PAGMaterial::PAGMaterial(const glm::vec4 & Kd, const glm::vec4 & Ks, float shininess)
{
	Kd_ = Kd;
	Ks_ = Ks;
	shininess_ = shininess;
}


PAGMaterial::~PAGMaterial()
{
}

void PAGMaterial::setKd(const glm::vec4 & Kd)
{
	Kd_ = Kd;
}

void PAGMaterial::setKs(const glm::vec4 & Ks)
{
	Ks_ = Ks;
}

void PAGMaterial::setShininess(float shininess)
{
	shininess_ = shininess;
}

glm::vec4 PAGMaterial::getKd()
{
	return Kd_;
}

glm::vec4 PAGMaterial::getKs()
{
	return Ks_;
}

float PAGMaterial::getShininess()
{
	return shininess_;
}

void PAGMaterial::applyKd(const PAGShaderProgram & shader_)
{
	shader_.setUniform("mtDiffuse", Kd_);
}

void PAGMaterial::applyKs(const PAGShaderProgram & shader_)
{
	shader_.setUniform("mtSpecular", Ks_);
}

void PAGMaterial::applyShininess(const PAGShaderProgram & shader_)
{
	shader_.setUniform("mtShininess", shininess_);
}

void PAGMaterial::apply(const PAGShaderProgram & shader_, bool Kd, bool Ks, bool shininess)
{
	if (Kd)
		applyKd(shader_);
	if (Ks)
		applyKs(shader_);
	if (shininess)
		applyShininess(shader_);
}
