#include "weldconstraint.h"
#include "basepart.h"

#include "ftapware/engine/client/memory/offsets.h"

namespace FTAPWare::Roblox {
    WeldConstraint::WeldConstraint(const Instance& instance) : Instance(instance) {}

    BasePart WeldConstraint::Part0() const {
        return BasePart(ReadFrom<uintptr_t>(Offsets::WeldConstraint::Part0), process);

    }

    BasePart WeldConstraint::Part1() const {
        return BasePart(ReadFrom<uintptr_t>(Offsets::WeldConstraint::Part1), process);
    }
}