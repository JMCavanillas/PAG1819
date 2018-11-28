#pragma once
#include <glm.hpp>
#include "PAGShaderProgram.h"

class PAGMaterial
{
	private:
		glm::vec4 Kd_;
		glm::vec4 Ks_;
		float shininess_;
	public:
		PAGMaterial();
		PAGMaterial(const glm::vec4 & Kd, const glm::vec4 & Ks, float shininess);
		virtual ~PAGMaterial();

		void setKd(const glm::vec4& Kd);
		void setKs(const glm::vec4& Ks);
		void setShininess(float shininess);

		glm::vec4 getKd();
		glm::vec4 getKs();
		float getShininess();

		void applyKd(const PAGShaderProgram& shader_);
		void applyKs(const PAGShaderProgram& shader_);
		void applyShininess(const PAGShaderProgram& shader_);

		void apply(const PAGShaderProgram& shader_, bool Kd = true, bool Ks = true, bool shininess = true);
};

