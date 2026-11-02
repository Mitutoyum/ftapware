#pragma once

#include "instance.h"

namespace FTAPWare::Roblox {
	class Humanoid : public Instance {
	public:
		using Instance::Instance;
		Humanoid(const Instance& instance);

		uint32_t GetFloorMaterial() const;
	};
}