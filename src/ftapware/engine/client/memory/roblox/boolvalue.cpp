#include "boolvalue.h"

#include "ftapware/engine/client/memory/offsets.h"

namespace FTAPWare::Roblox {
	BoolValue::BoolValue(const Instance& instance) : Instance(instance) {}

	bool BoolValue::Value() const {
		return ReadFrom<uint8_t>(Offsets::ValueBase::Value);
	}
}