#pragma once

#include "instance.h"

namespace FTAPWare::Roblox {
	class BasePart;

	class WeldConstraint : public Instance {
	public:
		using Instance::Instance;
		WeldConstraint(const Instance& instance);

		BasePart Part0() const;
		BasePart Part1() const;
	};
}