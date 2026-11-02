#pragma once

#include "matrix3x3.h"
#include "vector3.h"

namespace FTAPWare::Math {
    struct CFrame {
        Matrix3x3 rotation;
        Vector3 position;
    };
}