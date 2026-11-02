#pragma once

#include "instance.h"

namespace FTAPWare::Roblox {
    class InputObject;

    class MouseService : public Instance {
    public:
        using Instance::Instance;
        MouseService(const Instance& instance);

        InputObject GetInputObject() const;
    };
}