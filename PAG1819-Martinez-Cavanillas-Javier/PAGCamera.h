#pragma once
#include <glm.hpp>

class PAGCamera
{
	private:
		int viewportWidth_;
		int viewportHeight_;

		glm::vec3 position_;
		glm::vec3 up_;
		glm::vec3 u_;
		glm::vec3 n_;
		glm::vec3 lookAt_;

		float near_;
		float far_;
		float fov_;

		glm::mat4 mVision_;
		glm::mat4 mPerspective_;

		void initVision();
		void updateVision();

	public:
		PAGCamera();
		PAGCamera(const glm::vec3& position, const glm::vec3& lookAt, float near, float far, float fov);
		~PAGCamera();

		void orbit(float angle, bool onDegrees = true);
		void orbit_y(float angle, bool onDegrees = true);
		void pan(float angle, bool onDegrees = true);
		void tilt(float angle, bool onDegrees = true);
		void roll(float angle, bool onDegrees = true);

		void truck(float distance);
		void dolly(float distance);
		void boom(float distance);

		void zoom(int units);

		glm::vec3 getPosition();
		glm::vec3 getUp();
		glm::vec3 getU();
		glm::vec3 getLookAtDirection();
		glm::vec3 getLookAt();

		float getFar();
		float getNear();

		void setViewport(int width, int height);
		void setNear(float distance);
		void setFar(float distance);
		void setVisionAngle(float angle, bool onDegrees = true);

		void updatePerspective();
		glm::mat4 getPerspective();
		glm::mat4 getVision();
};

