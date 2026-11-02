#include "keybind_manager.h"

#include <windows.h>
#include <string>

namespace FTAPWare {
    void KeybindManager::RegisterKey(
        const std::string& id,
        int key,
        std::function<void()> callback
    ) {
        keybinds.insert_or_assign(
            id,
            KeyBind{ key, std::move(callback) }
        );
    }

    int KeybindManager::GetKey(const std::string& id) const {
        const auto it = keybinds.find(id);

        if (it == keybinds.end())
            return 0;

        return it->second.key;
    }

    void KeybindManager::SetKey(const std::string& id, int key) {
        const auto it = keybinds.find(id);

        if (it != keybinds.end())
            it->second.key = key;
    }

    void KeybindManager::Update() {
        for (auto& [id, bind] : keybinds) {
            if (bind.key != 0 &&
                bind.callback &&
                (GetAsyncKeyState(bind.key) & 1)) {
                bind.callback();
            }
        }
    }
}