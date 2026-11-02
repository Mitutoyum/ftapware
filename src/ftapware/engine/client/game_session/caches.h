#pragma once

#include <vector>

#include "ftapware/common/cache/lazycache.h"

namespace FTAPWare::Roblox {
    class VisualEngine;
    class BasePart;
    class Humanoid;
    class Players;
    class Player;
}

namespace FTAPWare::Math {
    struct Vector2;
}

namespace FTAPWare {
    class Caches {
    private:
        Cache::LazyCache<Roblox::Humanoid> humanoid;
        Cache::LazyCache<Math::Vector2> dimensions;
        Cache::LazyCache<Roblox::BasePart> humanoidrootpart;
        Cache::LazyCache<std::vector<Roblox::Player>> playerlist;
    public:
        Caches(
            const Roblox::VisualEngine& visualengine,
            const Roblox::Players& players,
            const Roblox::Player& localplayer
        );

        Cache::LazyCache<Roblox::Humanoid>& GetHumanoid();
        Cache::LazyCache<Math::Vector2>& GetDimensions();
        Cache::LazyCache<Roblox::BasePart>& GetHumanoidRootPart();
        Cache::LazyCache<std::vector<Roblox::Player>>& GetPlayerList();

    };
}