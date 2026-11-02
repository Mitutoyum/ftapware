#pragma once

#include "module.h"

#include <string>

namespace FTAPWare::Modules {
	class FOV : public Module {
	private:
		float fov { 70 };
	public:
		using Module::Module;
		
		std::string GetName() const override;
		void SetFOV(float FOV);
		float GetFOV() const;
	};
}