#include "mouseservice.h"
#include "inputobject.h"

#include "ftapware/engine/client/memory/offsets.h"

namespace FTAPWare::Roblox {
    MouseService::MouseService(const Instance& instance) : Instance(instance) {}

	InputObject MouseService::GetInputObject() const {
		uintptr_t inputobject = ReadFrom<uintptr_t>(Offsets::MouseService::InputObject);

		if (!inputobject)
			return InputObject();

		return InputObject(inputobject, process);
	}
}