#pragma once

namespace FTAPWare::Math {
    struct Vector3 {
        float x, y, z;

        static Vector3 NaN();
        float Magnitude() const;
        Vector3 Cross(const Vector3& other) const;
        Vector3 Normalize() const;
        bool IsValid() const;

        Vector3 operator-(const Vector3& other) const;
        Vector3 operator+(const Vector3& other) const;
        Vector3 operator*(float scalar) const ;
        bool operator==(const Vector3& other) const;
        bool operator!=(const Vector3& other) const;
    };
}