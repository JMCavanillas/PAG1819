#pragma once

#include <map>
#include "PAGMaterial.h"

class MaterialLibrary
{
	private:
		static MaterialLibrary* instance_;
		std::map<std::string, PAGMaterial> materials_;

		MaterialLibrary();
	public:
		static MaterialLibrary* getInstance();

		void setMaterial(std::string name, const PAGMaterial& material);
		
		PAGMaterial getMaterial(std::string name);

		virtual ~MaterialLibrary();
};

