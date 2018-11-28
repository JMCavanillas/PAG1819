#pragma once

#include <vector>
#include "PAG3DElement.h"

class PAG3DGroup : public PAG3DElement
{
	private:
		std::vector<PAG3DElement> elements_;
	public:
		PAG3DGroup();
		virtual ~PAG3DGroup();
};

