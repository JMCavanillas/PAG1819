#pragma once
#include <glm.hpp>

class PAGCamera
{
	private:
		int viewport_width_;
		int viewport_height_;

		glm::mat4 mVision_orig_;
		glm::vec3 lookAt_;

		float near_;
		float far_;
		float fov_;

		glm::mat4 mVision_;
		glm::mat4 mPerspective_;

	public:
		PAGCamera();
		PAGCamera(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& up, float near, float far, float fov);
		~PAGCamera();

		void orbit(float angle, bool onDegrees = true);
		void pan(float angle, bool onDegrees = true);
		void tilt(float angle, bool onDegrees = true);

		void truck(float distance);
		void dolly(float distance);
		void boom_crane(float distance);

		void zoom(int units);

		glm::vec3 getPosition();
		glm::vec3 getUp();
		glm::vec3 getU();
		glm::vec3 getLookAtDirection();
		glm::vec3 getLookAt();

		void setViewport(int width, int height);
		void setNear(float distance);
		void setFar(float distance);
		void setVisionAngle(float angle, bool onDegrees = true);

		void updatePerspective();
		glm::mat4 getPerspective();
		glm::mat4 getVision();
};

