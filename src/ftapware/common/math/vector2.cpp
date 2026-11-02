#include "vector2.h"

#include <cmath>
#include <limits>

namespace FTAPWare::Math {
    Vector2 Vector2::NaN() {
        return {
            std::numeric_limits<float>::quiet_NaN(),
            std::numeric_limits<float>::quiet_NaN()
        };
    }

    bool Vector2::IsValid() const {
        if (!std::isfinite(x) || !std::isfinite(y))
            return false;
        return true;
    }

    float Vector2::Magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2 Vector2::operator-(const Vector2& other) const {
        return { x - other.x, y - other.y};
    }
}