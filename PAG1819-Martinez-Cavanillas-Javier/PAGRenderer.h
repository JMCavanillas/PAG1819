#include <GL\glew.h>
#include <iostream>

#pragma once
class PAGRenderer
{
	PAGRenderer();

	static PAGRenderer* instance_;

	public:
		static PAGRenderer* getInstance();

		void refresh();
		void change_viewport_size(int width, int height);

		// M�todos provisionales para probar los callbacks - muestran solo informaci�n por consola
		void button_clicked(int button);
		void button_released(int button);
		void zoom(int cantidad);


		virtual ~PAGRenderer();
};

