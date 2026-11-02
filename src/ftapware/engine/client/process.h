#pragma once

#include <Windows.h>
#include <string>
#include <vector>

#include "ftapware/common/win32.h"

namespace FTAPWare {
    class Process {
    private:
        uint32_t PID;
        HANDLE handle;
        uintptr_t base_address;
    public:
        Process(uint32_t PID);
        ~Process();

        uintptr_t GetBaseAddress() const;
        uint32_t GetProcessId() const;
        HANDLE GetHandle() const;

        template <typename T>
        T Read(uintptr_t address) const {
            T buffer;

            NtReadVirtualMemory(handle, reinterpret_cast<PVOID>(address), &buffer, sizeof(buffer), nullptr);

            return buffer;
        }

        template <>
        inline std::string Read(uintptr_t address) const {
            uint64_t length = Read<uint64_t>(address + 0x10);

            address = (length >= 16) ? Read<uintptr_t>(address) : address;

            std::string buffer;
            buffer.resize(length);

            NtReadVirtualMemory(handle, reinterpret_cast<PVOID>(address), buffer.data(), length, nullptr);

            return buffer;
        }

        template <typename T>
        void Write(uintptr_t address, T buffer) const {
            NtWriteVirtualMemory(handle, reinterpret_cast<LPVOID>(address), &buffer, sizeof(T), nullptr);
        }

        template <>
        inline void Write<std::string>(uintptr_t address, std::string buffer) const {
            uintptr_t new_address = (Read<uint64_t>(address + 0x10) >= 16) ? Read<uintptr_t>(address) : address;

            size_t buffer_size = buffer.size();

            NtWriteVirtualMemory(handle, reinterpret_cast<PVOID>(new_address), reinterpret_cast<PVOID>(buffer.data()), buffer_size, nullptr);
            NtWriteVirtualMemory(handle, reinterpret_cast<PVOID>(address + 0x10), &buffer_size, sizeof(buffer_size), nullptr);
        }
    };
}