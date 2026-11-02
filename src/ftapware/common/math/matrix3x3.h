#pragma once

namespace FTAPWare::Math {
    struct Quaternion;

    struct Matrix3x3 {
        float data[9];

        Quaternion ToQuaternion() const;
        Matrix3x3 Lerp(const Matrix3x3& other, float alpha) const;
        Matrix3x3 Slerp(const Matrix3x3& target, float alpha) const;
    };
}