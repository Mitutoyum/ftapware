#include "visualengine.h"

#include "ftapware/common/math/vector2.h"
#include "ftapware/common/math/matrix4x4.h"
#include "ftapware/engine/client/memory/offsets.h"

namespace FTAPWare::Roblox {
    VisualEngine VisualEngine::Get(const Process& process) {
        uintptr_t visualengine = process.Read<uintptr_t>(process.GetBaseAddress() + Offsets::VisualEngine::Pointer);

        if (!visualengine) return VisualEngine();

        return VisualEngine(visualengine, &process);
    }

    Math::Vector2 VisualEngine::GetDimensions() const {
        return ReadFrom<Math::Vector2>(Offsets::VisualEngine::Dimensions);
    }

    Math::Matrix4x4 VisualEngine::GetViewMatrix() const {
        return ReadFrom<Math::Matrix4x4>(Offsets::VisualEngine::ViewMatrix);
    }
}