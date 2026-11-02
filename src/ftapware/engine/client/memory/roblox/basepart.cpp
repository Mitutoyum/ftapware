#include "basepart.h"
#include "primitive.h"

#include "ftapware/engine/client/memory/offsets.h"

namespace FTAPWare::Roblox {
	BasePart::BasePart(const Instance& instance) : Instance(instance) {}

	Primitive BasePart::GetPrimitive() const {
		uintptr_t primitive = ReadFrom<uintptr_t>(Offsets::BasePart::Primitive);

		if (!primitive)
			return Primitive();

		return Primitive(primitive, process);
	}
}