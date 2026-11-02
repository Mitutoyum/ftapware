#include "quaternion.h"
#include "matrix3x3.h"

#include <cmath>
#include <limits>
#include <algorithm>

namespace FTAPWare::Math {
    bool Quaternion::IsValid() const {
        if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z) || !std::isfinite(w))
            return false;
        return true;
    }

    float Quaternion::Magnitude() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    Quaternion Quaternion::Normalize() const {
        float magnitude = Magnitude();

        if (!magnitude) {
            return {
                0,
                0,
                0,
                1
            };
        }

        return { x / magnitude, y / magnitude, z / magnitude, w / magnitude };
    }

    Matrix3x3 Quaternion::ToMatrix3x3() const {
        Matrix3x3 m;
        float xx = x * x, yy = y * y, zz = z * z;
        float xy = x * y, xz = x * z, yz = y * z;
        float wx = w * x, wy = w * y, wz = w * z;

        m.data[0] = 1 - 2 * (yy + zz);
        m.data[1] = 2 * (xy - wz);
        m.data[2] = 2 * (xz + wy);

        m.data[3] = 2 * (xy + wz);
        m.data[4] = 1 - 2 * (xx + zz);
        m.data[5] = 2 * (yz - wx);

        m.data[6] = 2 * (xz - wy);
        m.data[7] = 2 * (yz + wx);
        m.data[8] = 1 - 2 * (xx + yy);

        return m;
    }

    float Quaternion::AngleTo(const Quaternion& other) const {
    float dot = x * other.x + y * other.y + z * other.z + w * other.w;

    dot = std::abs(dot);
    dot = std::min(1.0f, std::max(-1.0f, dot));

    return 2.0f * std::acos(dot);
}
}