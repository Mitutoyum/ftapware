#include <vector>

namespace FTAPWare::Roblox {
    class Player;
    class BasePart;
    class Instance;
}

namespace FTAPWare::Math {
    struct Vector2;
    struct Matrix4x4;
}

namespace FTAPWare {
    Roblox::Player GetClosestPlayerToMouse(const std::vector<Roblox::Player>& playerlist, Roblox::Player localplayer, Math::Matrix4x4 viewmatrix, Math::Vector2 dimensions, Math::Vector2 mouse_position, float max_magnitude = 100);
    Roblox::Player GetClosestPlayerToCharacter(const std::vector<Roblox::Player>& playerlist, Roblox::Player localplayer, Roblox::BasePart humanoidrootpart, float max_magnitude = 40);
    bool IsGrabbing(const Roblox::Instance& workspace);
}