#pragma once

#include <string>
#include <vector>
#include <functional>


namespace FTAPWare {
	// using KeybindCallback = std::function<void()>;

	// struct Keybind {
	// 	Keybind(std::string id);

	// 	std::string id;
	// 	int key = 0;
	// 	bool down = false;
	// 	KeybindCallback callback = nullptr;
	// };

	// class KeybindManager {
	// private:
	// 	std::vector<Keybind*> keybinds{};
	// public:
	// 	void Register(Keybind& keybind);
	// 	void Unregister(const Keybind& keybind);

	// 	void Poll();
	// };

    class KeybindManager {
    private:
        struct KeyBind {
            int key{};
            std::function<void()> callback;
        };

        std::unordered_map<std::string, KeyBind> keybinds;

    public:
        void RegisterKey(
            const std::string& id,
            int key,
            std::function<void()> callback
        );

        int GetKey(const std::string& id) const;
        void SetKey(const std::string& id, int key);
        void Update();
    };
}