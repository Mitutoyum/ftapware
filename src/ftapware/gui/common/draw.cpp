#include "draw.h"

#include <Windows.h>
#include <imgui.h>
#include <cstdio>
#include <string>

namespace FTAPWare::Draw {
    bool KeyBindButton(const char* label, int& key) {
        static bool waiting = false;
        static int* active = nullptr;

        ImGui::Text("%s:", label);
        ImGui::SameLine();

        char btnLabel[32];
        if (waiting && active == &key)
            snprintf(btnLabel, sizeof(btnLabel), "[Press any key]");
        else if (key)
            snprintf(btnLabel, sizeof(btnLabel), "[%c]", std::toupper(static_cast<unsigned char>(key)));
        else
            snprintf(btnLabel, sizeof(btnLabel), "[None]");

        std::string id = std::string(btnLabel) + "##" + label;

        bool changed = false;
        if (ImGui::Button(id.c_str())) {
            waiting = true;
            active = &key;
        }

        if (waiting && active == &key) {
            for (int vk = 8; vk < 256; vk++) {
                if (GetAsyncKeyState(vk) & 0x1) {
                    if (vk == VK_ESCAPE || vk == VK_DELETE || vk == VK_BACK) {
                        if (key != 0) {
                            key = 0;
                            changed = true;
                        }
                    } else {
                        ;
                        if (key != vk) {
                            key = vk;
                            changed = true;
                        }
                    }

                    waiting = false;
                    active = nullptr;
                    break;
                }
            }

            if (ImGui::IsMouseClicked(0) && !ImGui::IsItemHovered()) {
                waiting = false;
                active = nullptr;
            }
        }

        return changed;
    }
}