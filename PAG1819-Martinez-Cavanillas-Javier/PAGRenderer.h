#include <GL\glew.h>
#include <iostream>
#include<vector>
#include "PAGShaderProgram.h"
#include "PAGRevolutionObject.h"
#include "PAGCamera.h"

#pragma once
class PAGRenderer
{
		PAGRenderer();

		static PAGRenderer* instance_;

		PAGShaderProgram pointShader_;
		void pointShaderConfig(float point_size, const glm::vec3& color, const glm::mat4& view_matrix);
		PAGShaderProgram lineShader_;
		void lineShaderConfig(const glm::vec3& color, const glm::mat4& view_matrix);
		PAGShaderProgram triangleFaceShader_;
		void triangleFaceShaderConfig(const glm::mat4& view_matrix);
		PAGShaderProgram normalsAsColorShader_;
		void normalsAsColorShaderConfig(const glm::mat4& view_matrix);
		PAGShaderProgram tangentsAsColorShader_;
		void tangentsAsColorShaderConfig(const glm::mat4& view_matrix);
		PAGShaderProgram textCoordsAsColorShader_;
		void textCoordsAsColorsShaderConfig(const glm::mat4& view_matrix);
		PAGShaderProgram magicShader_;
		void magicShaderConfig(const glm::mat4& view_matrix);

		int viewport_height;
		int viewport_width;

		PAGCamera camera_;

		std::vector<PAGCamera> cameras_;
		unsigned currentCamera_;
		int currentView_;

		std::vector<PAGRevolutionObject> sceneObjects_;

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
		void orbit(float degrees);
		void zoom(int cantidad);


		virtual ~PAGRenderer();
};

