#pragma once

namespace FTAPWare::Math {
    struct Matrix3x3;

    struct Quaternion {
        float x, y, z, w;

        bool IsValid() const;
        float Magnitude() const;
        Quaternion Normalize() const;
        Matrix3x3 ToMatrix3x3() const;
        float AngleTo(const Quaternion& other) const;

        Quaternion operator*(float scalar) const { return { x * scalar, y * scalar, z * scalar, w * scalar }; }
        Quaternion operator+(const Quaternion& other) const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
    };
}