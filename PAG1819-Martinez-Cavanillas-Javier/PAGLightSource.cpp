#pragma once
#include "PAGLightSource.h"



PAGLightSource::PAGLightSource()
{
	applier_ = nullptr;

	ambient_ = diffuse_ = specular_ = glm::vec3(0.15);

	vMatrix_ = glm::mat4(1);
	position_ = glm::vec3(0);
	direction_ = glm::vec3(0,0,-1);

	scatterAngle_ = 20;
	scatterExp_ = 20;
}


void PAGLightSource::setApplier(PAGLightApplier * applier)
{
	applier_ = applier;
}

glm::mat4 PAGLightSource::getVision() const
{
	return vMatrix_;
}

glm::vec3 PAGLightSource::getAmbient() const
{
	return ambient_;
}

glm::vec3 PAGLightSource::getDiffuse() const
{
	return diffuse_;
}

glm::vec3 PAGLightSource::getSpecular() const
{
	return specular_;
}

glm::vec3 PAGLightSource::getPosition() const
{
	return position_;
}

glm::vec3 PAGLightSource::getDirection() const
{
	return direction_;
}

float PAGLightSource::getScatterAngle() const
{
	return scatterAngle_;
}

float PAGLightSource::getScatterExp() const
{
	return scatterExp_;
}

int PAGLightSource::getPreferedShader() const
{
	if (!applier_)
		return -1;

	return applier_->getPreferedShader();
}

void PAGLightSource::setVision(const glm::mat4 & vision_matrix)
{
	vMatrix_ = vision_matrix;
}

void PAGLightSource::setAmbient(const glm::vec3 & ambient)
{
	ambient_ = ambient;
}

void PAGLightSource::setDiffuse(const glm::vec3 & diffuse)
{
	diffuse_ = diffuse;
}

void PAGLightSource::setSpecular(const glm::vec3 & specular)
{
	specular_ = specular;
}

void PAGLightSource::setPosition(const glm::vec3 & position)
{
	position_ = position;
}

void PAGLightSource::setDirection(const glm::vec3 & direction)
{
	direction_ = direction;
}

void PAGLightSource::setScatterAngle(float angle)
{
	scatterAngle_ = angle;
}

void PAGLightSource::setScatterExp(float exp)
{
	scatterExp_ = exp;
}

void PAGLightSource::apply(const PAGShaderProgram & shader)
{
	applier_->apply(shader, *this);
}


PAGLightSource::~PAGLightSource()
{
}
