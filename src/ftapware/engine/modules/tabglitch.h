#pragma once

#include "module.h"

#include <string>

namespace FTAPWare::Modules {
	class TabGlitch : public Module {
	public:
		enum class Mode { Velocity, Position };

	private:
		Mode mode { Mode::Velocity };
	public:
		using Module::Module;

		std::string GetName() const override;
		void SetMode(Mode mode);
		Mode GetMode();

		void Trigger() const;
	};
}