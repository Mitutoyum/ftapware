#pragma once

#include <Windows.h>
#include <string>
#include <vector>

typedef LONG NTSTATUS;

extern "C" {
    __declspec(dllimport) NTSTATUS NTAPI NtReadVirtualMemory(
        HANDLE,
        PVOID,
        PVOID,
        SIZE_T,
        PSIZE_T
    );

    __declspec(dllimport) NTSTATUS NTAPI NtWriteVirtualMemory(
        HANDLE,
        PVOID,
        PVOID,
        SIZE_T,
        PSIZE_T
    );
}

namespace FTAPWare::Win32 {
    bool IsProcessNamed(uint32_t PID, const std::wstring& name);
    std::vector<uint32_t> FindProcessIds(const wchar_t* name);
    uintptr_t FindBaseAddress(uint32_t PID);
    HWND FindProcessWindow(uint32_t PID);
    bool LeftClick();
}