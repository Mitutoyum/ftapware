#include "process.h"

#include <string>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

namespace FTAPWare::Win32 {
    std::vector<uint32_t> FindProcessIds(const wchar_t* name) {
        std::vector<uint32_t> PIDs;

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if (snapshot == INVALID_HANDLE_VALUE) {
            return PIDs;
        }

        PROCESSENTRY32W entry = { sizeof(PROCESSENTRY32W) };

        if (Process32FirstW(snapshot, &entry)) {
            if (_wcsicmp(name, entry.szExeFile) == 0) {
                PIDs.push_back(entry.th32ProcessID);
            }
            while (Process32NextW(snapshot, &entry)) {
                if (_wcsicmp(name, entry.szExeFile) == 0) {
                    PIDs.push_back(entry.th32ProcessID);
                }
            }
        }

        CloseHandle(snapshot);
        return PIDs;
    }

    uintptr_t FindBaseAddress(uint32_t PID) {
        uintptr_t base_address = 0;
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, PID);

        if (snapshot != INVALID_HANDLE_VALUE) {
            MODULEENTRY32 module_entry = { sizeof(module_entry) };
            if (Module32First(snapshot, &module_entry)) {
                base_address = reinterpret_cast<uintptr_t>(module_entry.modBaseAddr);
            }
        }

        CloseHandle(snapshot);
        return base_address;
    }

    HWND FindProcessWindow(uint32_t PID) {
        struct HandleData {
            DWORD processId;
            HWND windowHandle;
        };

        HandleData data = { PID, nullptr };

        auto EnumWindowsCallback = [](HWND hwnd, LPARAM lParam) -> BOOL {
            HandleData& data = *reinterpret_cast<HandleData*>(lParam);
            DWORD windowPid = 0;
            GetWindowThreadProcessId(hwnd, &windowPid);

            if (windowPid == data.processId) {
                if (IsWindowVisible(hwnd) && GetWindow(hwnd, GW_OWNER) == nullptr) {
                    data.windowHandle = hwnd;
                    return FALSE;
                }
            }
            return TRUE;
            };

        EnumWindows(EnumWindowsCallback, reinterpret_cast<LPARAM>(&data));
        return data.windowHandle;
    }

    bool IsProcessNamed(uint32_t PID, const std::wstring& name) {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if (snapshot == INVALID_HANDLE_VALUE) {
            return false;
        }

        PROCESSENTRY32W entry = { sizeof(PROCESSENTRY32W) };
        bool found = false;

        if (Process32FirstW(snapshot, &entry)) {
            do {
                if (entry.th32ProcessID == PID) {
                    found = (_wcsicmp(entry.szExeFile, name.c_str()) == 0);
                    break;
                }
            } while (Process32NextW(snapshot, &entry));
        }

        CloseHandle(snapshot);
        return found;
    }

    bool LeftClick() {
        INPUT inputs[2] = {};

        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

        inputs[1].type = INPUT_MOUSE;
        inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

        return SendInput(2, inputs, sizeof(INPUT)) == 2;
    }
}