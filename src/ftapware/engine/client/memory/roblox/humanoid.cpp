#include "humanoid.h"

#include "ftapware/engine/client/memory/offsets.h"

namespace FTAPWare::Roblox {
    Humanoid::Humanoid(const Instance& instance) : Instance(instance) {}

    uint32_t Humanoid::GetFloorMaterial() const {
        return ReadFrom<uint32_t>(Offsets::Humanoid::FloorMaterial);
    }
}