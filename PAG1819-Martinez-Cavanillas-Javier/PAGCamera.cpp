#include "PAGCamera.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/matrix_access.hpp"
#include "gtc/constants.hpp"

// Inicializa la matriz de visión, (no necesita up)
void PAGCamera::initVision()
{
	n_ = glm::normalize(position_ - lookAt_);
	float eps = glm::epsilon<float>();
	glm::vec3 bound = glm::vec3(eps, eps, eps);
	glm::vec3 y_positive = glm::vec3(0, 1, 0);
	glm::vec3 z_positive = glm::vec3(0, 0, 1);

	bool equal_y_positive = glm::all(glm::greaterThan(n_, y_positive - bound)) 
		&& glm::all(glm::lessThan(n_, y_positive + bound));
	bool equal_y_negative = glm::all(glm::greaterThan(n_, -y_positive - bound))
		&& glm::all(glm::lessThan(n_, -y_positive + bound));

	if (equal_y_positive)
		u_ = glm::cross(z_positive, n_);
	else if (equal_y_negative)
		u_ = glm::cross(-z_positive, n_);
	else
		u_ = glm::cross(y_positive, n_);

	u_ = glm::normalize(u_);
	up_ = glm::cross(n_, u_);

	// Ya que hemos calculado todo formaremos la matriz manualmente
	mVision_ = glm::row(mVision_, 0, glm::vec4(u_, -glm::dot(u_, position_)));
	mVision_ = glm::row(mVision_, 1, glm::vec4(up_, -glm::dot(up_, position_)));
	mVision_ = glm::row(mVision_, 2, glm::vec4(n_, -glm::dot(n_, position_)));
	mVision_ = glm::row(mVision_, 3, glm::vec4(0, 0, 0, 1));

	//mVision_ = glm::lookAt(position_, lookAt_, up_);
}

// Actualiza la matriz de vision
void PAGCamera::updateVision()
{
	n_ = glm::normalize(position_ - lookAt_);
	up_ = glm::cross(n_, u_);
	u_ = glm::cross(up_, n_);

	// Ya que hemos calculado todo formaremos la matriz manualmente
	mVision_ = glm::row(mVision_, 0, glm::vec4(u_, -glm::dot(u_, position_)));
	mVision_ = glm::row(mVision_, 1, glm::vec4(up_, -glm::dot(up_, position_)));
	mVision_ = glm::row(mVision_, 2, glm::vec4(n_, -glm::dot(n_, position_)));
	mVision_ = glm::row(mVision_, 3, glm::vec4(0, 0, 0, 1));

	//mVision_ = glm::lookAt(position_, lookAt_, up_);
}

// Public Methods
PAGCamera::PAGCamera()
{
	position_ = glm::vec3(0, 0, 17);
	lookAt_ = glm::vec3(0, 0, 0);

	viewportWidth_ = 1080;
	viewportHeight_ = 720;

	near_ = 1.0f;
	far_ = 300.0f;
	setVisionAngle(60.0f);

	initVision();
	updatePerspective();
}

PAGCamera::PAGCamera(const glm::vec3& position, const glm::vec3& lookAt, float near, float far, float fovX)
{
	position_ = position;
	lookAt_ = lookAt;

	viewportWidth_ = 1080;
	viewportHeight_ = 720;

	near_ = near;
	far_ = far;
	setVisionAngle(fovX);

	initVision();
	updatePerspective();
}


PAGCamera::~PAGCamera()
{
}

void PAGCamera::orbit(float angle, bool onDegrees)
{
	if (onDegrees)
		angle = glm::radians(angle);

	glm::mat4 identity = glm::mat4(1);

	glm::mat4 transform = identity;
	transform = glm::rotate(identity, angle, up_);
	position_ = glm::vec3(transform * glm::vec4(position_, 1));
	u_ = glm::normalize(glm::vec3(transform * glm::vec4(u_, 1)));

	updateVision();
}

void PAGCamera::orbit_y(float angle, bool onDegrees)
{
	if (onDegrees)
		angle = glm::radians(angle);

	glm::mat4 identity = glm::mat4(1);

	glm::mat4 transform = identity;
	transform = glm::rotate(identity, angle, glm::vec3(0,1,0));
	position_ = glm::vec3(transform * glm::vec4(position_, 1));
	u_ = glm::normalize(glm::vec3(transform * glm::vec4(u_, 1)));

	updateVision();
}

void PAGCamera::pan(float angle, bool onDegrees)
{
	if (onDegrees)
		angle = glm::radians(angle);
 
	glm::mat4 identity = glm::mat4(1);
	
	glm::mat4 rotation = glm::rotate(identity, angle, up_);
	glm::mat4 transform = identity;
	transform = glm::translate(identity, -position_); 
	transform = rotation * transform;
	transform = glm::translate(identity, position_) * transform;

	lookAt_ = glm::vec3(transform * glm::vec4(lookAt_, 1));
	u_ = glm::normalize(glm::vec3(rotation * glm::vec4(u_, 1)));
	updateVision();
}

void PAGCamera::tilt(float angle, bool onDegrees)
{
	if (onDegrees)
		angle = glm::radians(angle);

	glm::mat4 identity = glm::mat4(1);

	glm::mat4 transform = identity;
	transform = glm::translate(identity, -position_);
	transform = glm::rotate(identity, angle, u_) * transform;
	transform = glm::translate(identity, position_) * transform;

	lookAt_ = glm::vec3(transform * glm::vec4(lookAt_, 1));
	updateVision();
}

void PAGCamera::roll(float angle, bool onDegrees)
{
	if (onDegrees)
		angle = glm::radians(angle);

	glm::mat4 identity = glm::mat4(1);

	glm::mat4 rotation = glm::rotate(identity, angle, n_);
	u_ = glm::normalize(glm::vec3(rotation * glm::vec4(u_, 1)));

	updateVision();
}

void PAGCamera::truck(float distance)
{
	glm::mat4 identity = glm::mat4(1);
	glm::mat4 transform = identity;
	
	glm::vec3 translation = u_ * distance;
	transform = glm::translate(identity, translation);
	lookAt_ = glm::vec3(transform * glm::vec4(lookAt_, 1));
	position_ = glm::vec3(transform * glm::vec4(position_, 1));

	updateVision();
}

void PAGCamera::dolly(float distance)
{
	glm::mat4 identity = glm::mat4(1);
	glm::mat4 transform = identity;

	glm::vec3 translation = n_ * distance;
	transform = glm::translate(identity, translation);
	//lookAt_ = glm::vec3(transform * glm::vec4(lookAt_, 1));
	glm::vec3 position = glm::vec3(transform * glm::vec4(position_, 1));
	if (glm::distance(position, lookAt_) > (near_ + glm::epsilon<float>() + distance) )
		position_ = position;

	updateVision();
}

void PAGCamera::boom(float distance)
{
	glm::mat4 identity = glm::mat4(1);
	glm::mat4 transform = identity;

	glm::vec3 translation = up_ * distance;
	transform = glm::translate(identity, translation);
	lookAt_ = glm::vec3(transform * glm::vec4(lookAt_, 1));
	position_ = glm::vec3(transform * glm::vec4(position_, 1));

	updateVision();
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
	return position_;
}

glm::vec3 PAGCamera::getUp()
{
	return up_;
}

glm::vec3 PAGCamera::getU()
{
	return u_;
}

glm::vec3 PAGCamera::getLookAtDirection()
{
	return -n_;
}

glm::vec3 PAGCamera::getLookAt()
{
	return lookAt_;
}

float PAGCamera::getFar()
{
	return far_;
}

float PAGCamera::getNear()
{
	return near_;
}

void PAGCamera::setViewport(int width, int height)
{
	viewportWidth_ = width;
	viewportHeight_ = height;
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

	fov_ = 2 * glm::atan(glm::tan(angle / 2) / (float(viewportWidth_) / viewportHeight_));
	updatePerspective();
}


void PAGCamera::updatePerspective()
{
	mPerspective_ = glm::perspective(fov_, float(viewportWidth_) / viewportHeight_, near_, far_);
}

glm::mat4 PAGCamera::getPerspective()
{
	return mPerspective_;
}

glm::mat4 PAGCamera::getVision()
{
	return mVision_;
}
