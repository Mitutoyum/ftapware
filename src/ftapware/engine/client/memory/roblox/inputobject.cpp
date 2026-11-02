#include "inputobject.h"

#include "ftapware/common/math/vector2.h"
#include "ftapware/engine/client/memory/offsets.h"

#include <iostream>

namespace FTAPWare::Roblox {
    InputObject::InputObject(const Instance& instance) : Instance(instance) {}

    Math::Vector2 InputObject::GetPosition() const {
        return ReadFrom<Math::Vector2>(Offsets::InputObject::MousePosition);
    }
}