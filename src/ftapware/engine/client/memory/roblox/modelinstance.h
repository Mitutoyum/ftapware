#pragma once

#include "instance.h"

namespace FTAPWare::Roblox {
	class BasePart;
	class Humanoid;

	class ModelInstance : public Instance {
	public:
		using Instance::Instance;
		ModelInstance(const Instance& instance);

		BasePart GetHumanoidRootPart() const;
		Humanoid GetHumanoid() const;
	};
}