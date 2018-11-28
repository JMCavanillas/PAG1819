#include "PAGCamera.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/matrix_access.hpp"

// Public Methods
PAGCamera::PAGCamera()
{
	glm::vec3 position_ = glm::vec3(0, 3, 15);
	lookAt_ = glm::vec3(0, 3, 0);
	glm::vec3 up_ = glm::vec3(0, 1, 0);

	viewport_width_ = 1080;
	viewport_height_ = 720;

	near_ = 1.0f;
	far_ = 300.0f;
	fov_ = glm::radians(50.0f);

	mVision_ = glm::lookAt(position_, lookAt_, up_);
	updatePerspective();
}

PAGCamera::PAGCamera(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& up, float near, float far, float fovX)
{
	glm::vec3 position_ = position;
	glm::vec3 lookAt_ = lookAt;
	glm::vec3 up_ = up;

	viewport_width_ = 1080;
	viewport_height_ = 720;

	near_ = near;
	far_ = far;
	setVisionAngle(fovX);

	mVision_ = glm::lookAt(position_, lookAt_, up_);
	updatePerspective();
}


PAGCamera::~PAGCamera()
{
}

void PAGCamera::orbit(float angle, bool onDegrees)
{
	if (onDegrees)
		angle = glm::radians(angle);

	glm::vec3 position_ = getPosition();

	glm::mat4 transform = glm::translate(glm::mat4(1), position_);
	transform = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * transform;
	transform = glm::translate(glm::mat4(1), -position_) * transform;
	
	mVision_ = transform * mVision_;
}

void PAGCamera::pan(float angle, bool onDegrees)
{
	if (onDegrees)
		angle = glm::radians(angle);
 
	mVision_ = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * mVision_;
}

void PAGCamera::tilt(float angle, bool onDegrees)
{
	if (onDegrees)
		angle = glm::radians(angle);

	mVision_ = glm::rotate(glm::mat4(1), -angle, glm::vec3(1, 0, 0)) * mVision_;
}

void PAGCamera::truck(float distance)
{
	glm::mat3 rotation_matrix_ = glm::mat3(mVision_);
	glm::vec3 displacement = glm::vec3(distance, 0, 0);
	displacement = rotation_matrix_ * displacement;
	glm::mat4 translation_matrix = glm::translate(glm::mat4(1), displacement);

	mVision_ = translation_matrix * mVision_;
	lookAt_ = glm::vec3(translation_matrix * glm::vec4(lookAt_, 1));
}

void PAGCamera::dolly(float distance)
{
	glm::mat3 rotation_matrix_ = glm::mat3(mVision_);
	glm::vec3 displacement = glm::vec3(0, 0, distance);
	displacement = rotation_matrix_ * displacement;
	glm::mat4 translation_matrix = glm::translate(glm::mat4(1), displacement);

	mVision_ = translation_matrix * mVision_;
	lookAt_ = glm::vec3(translation_matrix * glm::vec4(lookAt_, 1));
}

void PAGCamera::boom_crane(float distance)
{
	glm::mat3 rotation_matrix_ = glm::mat3(mVision_);
	glm::vec3 displacement = glm::vec3(0, distance, 0);
	displacement = rotation_matrix_ * displacement;
	glm::mat4 translation_matrix = glm::translate(glm::mat4(1), displacement);

	mVision_ = translation_matrix * mVision_;
	lookAt_ = glm::vec3(translation_matrix * glm::vec4(lookAt_, 1));
}

void PAGCamera::zoom(int units)
{
	float factor = 2 * units;
	float fov = fov_ - glm::radians(factor);
	
	if (fov >= 0 && fov < glm::pi<float>())
		fov_ = fov;

	updatePerspective();
}

glm::vec3 PAGCamera::getPosition()
{
	return -glm::vec3(glm::column(mVision_, 3));
}

glm::vec3 PAGCamera::getUp()
{
	return glm::vec3(glm::row(mVision_orig_, 1));
}

glm::vec3 PAGCamera::getU()
{
	return glm::vec3(glm::row(mVision_, 0));
}

glm::vec3 PAGCamera::getLookAtDirection()
{
	return glm::vec3(glm::row(mVision_, 2));
}

glm::vec3 PAGCamera::getLookAt()
{
	return lookAt_;
}

void PAGCamera::setViewport(int width, int height)
{
	viewport_width_ = width;
	viewport_height_ = height;
	updatePerspective();
}

void PAGCamera::setNear(float distance)
{
	near_ = distance;
	updatePerspective();
}

void PAGCamera::setFar(float distance)
{
	far_ = distance;
	updatePerspective();
}

void PAGCamera::setVisionAngle(float angle, bool onDegrees)
{
	if (onDegrees)
		angle = glm::radians(angle);

	fov_ = 2 * glm::atan(glm::tan(angle / 2) / (float(viewport_width_) / viewport_height_));
	updatePerspective();
}


void PAGCamera::updatePerspective()
{
	mPerspective_ = glm::perspective(fov_, float(viewport_width_) / viewport_height_, near_, far_);
}

glm::mat4 PAGCamera::getPerspective()
{
	return mPerspective_;
}

glm::mat4 PAGCamera::getVision()
{
	return mVision_;
}
