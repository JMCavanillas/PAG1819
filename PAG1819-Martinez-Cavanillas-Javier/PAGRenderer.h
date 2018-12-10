#pragma once

#include <GL\glew.h>
#include <iostream>
#include <vector>

#include "PAGEnumeration.h"
#include "PAGShaderCollection.h"
#include "PAGShaderProgram.h"
#include "PAGRevolutionObject.h"
#include "PAGCamera.h"
#include "PAGLightSource.h"

#include "PAGSpotLightApplier.h"
#include "PAGPointLightApplier.h"
#include "PAGDirLightApplier.h"
#include "PAGAmbientLightApplier.h"

#include "PAG3DGroup.h"


class PAGRenderer
{
		PAGRenderer();

		static PAGRenderer* instance_;

		int viewport_height;
		int viewport_width;

		PAGShaderCollection shaders_;
		std::vector<PAGCamera> cameras_;
		unsigned currentCamera_;
		int currentView_;

		PAG3DGroup sceneObjects_;

		PAGPointLightApplier pointLightApplier_;
		PAGDirLightApplier dirLightApplier_;
		PAGAmbientLightApplier ambientLightApplier_;
		PAGSpotLightApplier spotLightApplier_;
		
		std::vector<PAGLightSource> lights_;

	public:
		static PAGRenderer* getInstance();

		void refresh();
		void change_viewport_size(int width, int height);
		void prepareOpenGL();

		// Métodos provisionales para probar los callbacks - muestran solo información por consola

		void nextView();
		void nextCamera();
		void setCamera(unsigned n_camera);
		void setView(int view);
		void button_clicked(int button);
		void button_released(int button);

		void tilt(float degrees);
		void pan(float degree);
		void truck(float distance);
		void boom(float distance);
		void dolly(float distance);
		void orbit(float degrees);
		void zoom(int cantidad);


		virtual ~PAGRenderer();
};

