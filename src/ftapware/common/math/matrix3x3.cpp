#include "matrix3x3.h"
#include "quaternion.h"

#include <cmath>
#include <limits>
#include <algorithm>

namespace FTAPWare::Math {
    Quaternion Matrix3x3::ToQuaternion() const {
        Quaternion q;
        float trace = data[0] + data[4] + data[8];
        if (trace > 0.0f) {
            float s = std::sqrt(trace + 1.0f) * 2.0f;
            q.w = 0.25f * s;
            q.x = (data[7] - data[5]) / s;
            q.y = (data[2] - data[6]) / s;
            q.z = (data[3] - data[1]) / s;
        }
        else {
            if (data[0] > data[4] && data[0] > data[8]) {
                float s = std::sqrt(1.0f + data[0] - data[4] - data[8]) * 2.0f;
                q.w = (data[7] - data[5]) / s;
                q.x = 0.25f * s;
                q.y = (data[1] + data[3]) / s;
                q.z = (data[2] + data[6]) / s;
            }
            else if (data[4] > data[8]) {
                float s = std::sqrt(1.0f + data[4] - data[0] - data[8]) * 2.0f;
                q.w = (data[2] - data[6]) / s;
                q.x = (data[1] + data[3]) / s;
                q.y = 0.25f * s;
                q.z = (data[5] + data[7]) / s;
            }
            else {
                float s = std::sqrt(1.0f + data[8] - data[0] - data[4]) * 2.0f;
                q.w = (data[3] - data[1]) / s;
                q.x = (data[2] + data[6]) / s;
                q.y = (data[5] + data[7]) / s;
                q.z = 0.25f * s;
            }
        }
        return q.Normalize();
    }

    Matrix3x3 Matrix3x3::Lerp(const Matrix3x3& other, float alpha) const {
        Matrix3x3 result;
        float it = 1.0f - alpha;

        for (int i = 0; i < 9; i++)
            result.data[i] = it * data[i] + alpha * other.data[i];

        return result;
    }

    Matrix3x3 Matrix3x3::Slerp(const Matrix3x3& target, float alpha) const {
        Quaternion qCurrent = ToQuaternion();
        Quaternion qTarget = target.ToQuaternion();

        if (!qCurrent.IsValid()) qCurrent = { 0,0,0,1 };
        if (!qTarget.IsValid())  qTarget = { 0,0,0,1 };

        float dot = qCurrent.x * qTarget.x + qCurrent.y * qTarget.y + qCurrent.z * qTarget.z + qCurrent.w * qTarget.w;
        Quaternion b = qTarget;
        if (dot < 0.0f) { b = { -b.x, -b.y, -b.z, -b.w }; dot = -dot; }

        const float DOT_THRESHOLD = 0.9995f;
        Quaternion qResult;
        if (dot > DOT_THRESHOLD) {
            qResult = qCurrent * (1.0f - alpha) + b * alpha;
            qResult = qResult.Normalize();
        }
        else {
            float theta_0 = std::acos(dot);
            float theta = theta_0 * alpha;
            float sin_theta = std::sin(theta);
            float sin_theta_0 = std::sin(theta_0);

            float s0 = std::cos(theta) - dot * sin_theta / sin_theta_0;
            float s1 = sin_theta / sin_theta_0;

            qResult = qCurrent * s0 + b * s1;
            qResult = qResult.Normalize();
        }

        return qResult.ToMatrix3x3();
    }
}