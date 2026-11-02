#pragma once

#include "instance.h"

namespace FTAPWare::Roblox {
	class BoolValue : public Instance {
	public:
		using Instance::Instance;
		BoolValue(const Instance& instance);

		bool Value() const;
	};
}