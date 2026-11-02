#pragma once

#include "ftapware/engine/client/process.h"

namespace FTAPWare {
	class RemoteObject {
	protected:
		uintptr_t address = 0;
		const Process* process = nullptr;
	public:
        RemoteObject() = default;
        RemoteObject(uintptr_t address, const Process* process);

		uintptr_t Self() const;
		virtual bool IsValid() const;

		template <typename T>
		T ReadFrom(uintptr_t offset) const {
			return process->Read<T>(address + offset);
		}

		template <typename T>
		void WriteTo(uintptr_t offset, T buffer) const {
			process->Write<T>(address + offset, buffer);
		}
	};
}