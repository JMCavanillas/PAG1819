#pragma once

class PAGLightApplier;
#include <glm.hpp>
#include "PAGLightApplier.h"
#include "PAGShaderProgram.h"

class PAGLightSource
{
	private:
		glm::mat4 vMatrix_;

		glm::vec3 ambient_;
		glm::vec3 diffuse_;
		glm::vec3 specular_;

		glm::vec3 position_;
		glm::vec3 direction_;
		float scatterAngle_;
		float scatterExp_;
		
		PAGLightApplier* applier_;
	public:
		PAGLightSource();
		//PAGLightSource(PAGLightApplier* applier);

		void setApplier(PAGLightApplier* applier);

		glm::mat4 getVision() const;

		glm::vec3 getAmbient() const;
		glm::vec3 getDiffuse() const;
		glm::vec3 getSpecular() const;
			 
		glm::vec3 getPosition() const;
		glm::vec3 getDirection() const;
		float getScatterAngle() const;
		float getScatterExp() const;

		int getPreferedShader() const;

		void setVision(const glm::mat4& vision_matrix);

		void setAmbient(const glm::vec3& ambient);
		void setDiffuse(const glm::vec3& diffuse);
		void setSpecular(const glm::vec3& specular);

		void setPosition(const glm::vec3& position);
		void setDirection(const glm::vec3& direction);
		void setScatterAngle(float angle);
		void setScatterExp(float exp);

		void apply(const PAGShaderProgram& shader);

		virtual ~PAGLightSource();
};

