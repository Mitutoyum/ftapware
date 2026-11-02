#pragma once

#include "instance.h"

namespace FTAPWare::Math {
	struct Vector2;
}

namespace FTAPWare::Roblox {
	class InputObject : public Instance {
	public:
		using Instance::Instance;
		InputObject(const Instance& instance);

		Math::Vector2 GetPosition() const;
	};
}