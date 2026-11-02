#pragma once

#include "instance.h"

namespace FTAPWare::Roblox {
    class Player;

    class Players : public Instance {
    public:
        using Instance::Instance;
        Players(const Instance& instance);

        Player GetLocalPlayer() const;
        std::vector<Player> GetPlayers() const;
    };
}