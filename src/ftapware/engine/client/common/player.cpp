#include "player.h"

#include "ftapware/common/math/utils.h"
#include "ftapware/common/math/vector2.h"
#include "ftapware/common/math/vector3.h"
#include "ftapware/common/math/matrix4x4.h"

#include "ftapware/engine/client/memory/roblox/player.h"
#include "ftapware/engine/client/memory/roblox/basepart.h"
#include "ftapware/engine/client/memory/roblox/instance.h"
#include "ftapware/engine/client/memory/roblox/primitive.h"
#include "ftapware/engine/client/memory/roblox/modelinstance.h"

#include <iostream>

namespace FTAPWare {
    Roblox::Player GetClosestPlayerToMouse(const std::vector<Roblox::Player>& playerlist, Roblox::Player localplayer, Math::Matrix4x4 viewmatrix, Math::Vector2 dimensions, Math::Vector2 mouse_position, float max_magnitude) {
        Roblox::Player closest_player;
        float closest_distance = max_magnitude;

        if (playerlist.empty())
            return closest_player;

        for (const auto& player : playerlist) {
            if (player.Self() == localplayer.Self()) continue;

            auto character = player.GetCharacter();
            if (!character.IsValid()) continue;

            auto humanoidrootpart = character.GetHumanoidRootPart();
            if (!humanoidrootpart.IsValid()) continue;

            Math::Vector2 screen_position = Math::WorldToScreenPoint(humanoidrootpart.GetPrimitive().GetPosition(), viewmatrix, dimensions);
            if (!screen_position.IsValid()) continue;

            float magnitude = (mouse_position - screen_position).Magnitude();
            if (magnitude > max_magnitude) continue;

            if (closest_distance > magnitude) {
                closest_distance = magnitude;
                closest_player = player;
            }
        }
        
        return closest_player;
    }

    Roblox::Player GetClosestPlayerToCharacter(const std::vector<Roblox::Player>& playerlist, Roblox::Player localplayer, Roblox::BasePart humanoidrootpart, float max_magnitude) {
        Roblox::Player closest_player;
        float closest_distance = max_magnitude;

        if (playerlist.empty())
            return closest_player;

        for (const auto& player : playerlist) {
            if (player.Self() == localplayer.Self()) continue;

            auto player_character = player.GetCharacter();
            if (!player_character.IsValid()) continue;

            auto player_humanoidrootpart = player_character.GetHumanoidRootPart();
            if (!player_humanoidrootpart.IsValid()) continue;

            float magnitude = (humanoidrootpart.GetPrimitive().GetPosition() - player_humanoidrootpart.GetPrimitive().GetPosition()).Magnitude();
            if (magnitude > max_magnitude) continue;

            if (closest_distance > magnitude) {
                closest_distance = magnitude;
                closest_player = player;
            }
        }
 
       return closest_player;
    }

    bool IsGrabbing(const Roblox::Instance& workspace) {
        auto grabparts = workspace.FindFirstChild("GrabParts");

        if (grabparts.IsValid())
            return true;
        return false;
    }
}