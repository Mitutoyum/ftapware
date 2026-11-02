#include "remote_object.h"

namespace FTAPWare {
	RemoteObject::RemoteObject(uintptr_t address, const Process* process) : address(address), process(process) {}

	uintptr_t RemoteObject::Self() const {
		return address;
	}

	bool RemoteObject::IsValid() const {
		if (!address)
			return false;
		return true;
	}
}