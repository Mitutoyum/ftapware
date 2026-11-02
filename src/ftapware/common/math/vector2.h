#pragma once

namespace FTAPWare::Math {
    struct Vector2 {
        float x, y;

        static Vector2 NaN();
        bool IsValid() const;
        float Magnitude() const;
        Vector2 operator-(const Vector2& other) const;
    };
}