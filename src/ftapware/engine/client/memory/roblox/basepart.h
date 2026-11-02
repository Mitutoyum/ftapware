#pragma once

#include "instance.h"

namespace FTAPWare::Roblox {
	class Primitive;

	class BasePart : public Instance {
	public:
		using Instance::Instance;
		BasePart(const Instance& instance);

		Primitive GetPrimitive() const;
	};
}