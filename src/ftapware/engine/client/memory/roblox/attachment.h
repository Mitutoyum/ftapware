#pragma once

#include "instance.h"

namespace FTAPWare::Roblox {
	class Attachment : public Instance {
	public:
		using Instance::Instance;
		Attachment(const Instance& instance);

		//Vector3 Position() const;
		//Vector3 WorldPosition() const;

	};
}