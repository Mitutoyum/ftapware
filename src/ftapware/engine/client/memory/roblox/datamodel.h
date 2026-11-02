#pragma once

#include "instance.h"

namespace FTAPWare::Roblox {
	class DataModel : public Instance {
	public:
		using Instance::Instance;
		DataModel(const Instance& instance);

		static DataModel Get(const Process& process);
		bool IsGameLoaded() const;
	};
}