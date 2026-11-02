#pragma once

#include "instance.h"

namespace FTAPWare::Roblox {
	class ModelInstance;

	class Player : public Instance {
	public:
		using Instance::Instance;
		Player(const Instance& instance);

		ModelInstance GetCharacter() const;
	};
}