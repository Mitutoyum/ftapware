#include "vector3.h"

#include <cmath>
#include <limits>
#include <algorithm>

namespace FTAPWare::Math {
    Vector3 Vector3::NaN() {
        return {
            std::numeric_limits<float>::quiet_NaN(),
            std::numeric_limits<float>::quiet_NaN(),
            std::numeric_limits<float>::quiet_NaN()
        };
    }

    float Vector3::Magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 Vector3::Cross(const Vector3& other) const {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        };
    }

    bool Vector3::IsValid() const {
        if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z))
            return false;
        return true;
    }

    Vector3 Vector3::Normalize() const {
        float magnitude = Magnitude();
        if (magnitude == 0)
            return { 0, 0, 0 };
        return { x / magnitude, y / magnitude, z / magnitude };
    }

    Vector3 Vector3::operator-(const Vector3& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }

    Vector3 Vector3::operator+(const Vector3& other) const {
        return { x + other.x, y + other.y, z + other.z };
    }

    Vector3 Vector3::operator*(float scalar) const {
        return { x * scalar, y * scalar, z * scalar };
    }

    bool Vector3::operator==(const Vector3& other) const {
        const float EPSILON = 1e-5f;

        auto compare = [EPSILON](float a, float b) -> bool {
            float diff = std::fabs(a - b);
            float scale = (std::max)(1.0f, (std::max)(std::fabs(a), std::fabs(b)));
            return diff <= EPSILON * scale;
            };

        return compare(x, other.x) && compare(y, other.y) && compare(z, other.z);
    }

    bool Vector3::operator!=(const Vector3& other) const {
        return !(*this == other);
    }
}