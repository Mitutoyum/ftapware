#include "players.h"

#include "player.h"
#include "ftapware/engine/client/memory/offsets.h"

#include <iostream>



namespace FTAPWare::Roblox {
    Players::Players(const Instance& instance) : Instance(instance) {}

	Player Players::GetLocalPlayer() const {
		uintptr_t localplayer = ReadFrom<uintptr_t>(Offsets::Players::LocalPlayer);

		if (!localplayer)
			return Player();

		return Player(localplayer, process);
	}

	std::vector<Player> Players::GetPlayers() const {
		auto children = GetChildren();
		std::vector<Player> players;

		for (const auto& child : children) {
			players.push_back(Player(child));
		}

		return players;
	}
}