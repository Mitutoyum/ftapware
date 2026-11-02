#include "caches.h"

#include "ftapware/common/math/vector2.h"

#include "ftapware/engine/client/memory/roblox/player.h"
#include "ftapware/engine/client/memory/roblox/players.h"
#include "ftapware/engine/client/memory/roblox/humanoid.h"
#include "ftapware/engine/client/memory/roblox/basepart.h"
#include "ftapware/engine/client/memory/roblox/visualengine.h"
#include "ftapware/engine/client/memory/roblox/modelinstance.h"

namespace FTAPWare {
    Caches::Caches(
        const Roblox::VisualEngine& visualengine,
        const Roblox::Players& players,
        const Roblox::Player& localplayer
    ) :
    playerlist(
        std::chrono::seconds(2),
        [&players]() -> std::vector<Roblox::Player> {
            auto playerlist = players.GetPlayers();

            return playerlist;
        }
    ),

    dimensions(
        std::chrono::seconds(2),
        [&visualengine]() -> Math::Vector2 {
            return visualengine.GetDimensions();
        }
    ),

    humanoidrootpart(
        std::chrono::seconds(1),
        [&localplayer]() -> Roblox::BasePart {
            auto character = localplayer.GetCharacter();

            if (!character.IsValid())
                return Roblox::BasePart();

            auto humanoidrootpart = character.GetHumanoidRootPart();

            if (!humanoidrootpart.IsValid())
                return Roblox::BasePart();

            return humanoidrootpart;
        }
    ),

    humanoid(
        std::chrono::seconds(1),
        [&localplayer]() -> Roblox::Humanoid {
            auto character = localplayer.GetCharacter();

            if (!character.IsValid())
                return Roblox::Humanoid();

            auto humanoid = character.GetHumanoid();

            if (!humanoid.IsValid())
                return Roblox::Humanoid();

            return humanoid;
        }
    ) {}

    Cache::LazyCache<Roblox::Humanoid>& Caches::GetHumanoid() {
        return humanoid;
    }

    Cache::LazyCache<Math::Vector2>& Caches::GetDimensions() {
        return dimensions;
    }

    Cache::LazyCache<Roblox::BasePart>& Caches::GetHumanoidRootPart() {
        return humanoidrootpart;
    }

    Cache::LazyCache<std::vector<Roblox::Player>>& Caches::GetPlayerList() {
        return playerlist;
    }
}