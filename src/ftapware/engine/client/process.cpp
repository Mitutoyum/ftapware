#include "process.h"

#include "ftapware/common/win32.h"

#include <Windows.h>
#include <TlHelp32.h>
#include <stdexcept>

namespace FTAPWare {
	Process::Process(uint32_t PID) : PID(PID), handle(nullptr), base_address(0) {
		handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

		if (!handle) {
			throw std::runtime_error(
				"Failed to open process " + std::to_string(PID)
			);
		}

		base_address = Win32::FindBaseAddress(PID);

		if (!base_address) {
			CloseHandle(handle);
			handle = nullptr;

			throw std::runtime_error(
				"Failed to find base address of " + std::to_string(PID)
			);
		}
	}

	Process::~Process() {
		if (handle) {
			CloseHandle(handle);
		}
	}

	uintptr_t Process::GetBaseAddress() const {
		return base_address;
	}

	HANDLE Process::GetHandle() const {
		return handle;
	}

	uint32_t Process::GetProcessId() const {
		return PID;
	}
}