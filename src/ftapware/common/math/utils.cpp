#include "utils.h"

#include "cframe.h"
#include "vector2.h"
#include "vector3.h"
#include "matrix4x4.h"
#include "quaternion.h"

namespace FTAPWare::Math {
	Vector2 WorldToScreenPoint(const Vector3& position, const Matrix4x4& viewmatrix, const Vector2& dimensions) {
		Quaternion quaternion;

		quaternion.x = (position.x * viewmatrix.data[0]) + (position.y * viewmatrix.data[1]) + (position.z * viewmatrix.data[2]) + viewmatrix.data[3];
		quaternion.y = (position.x * viewmatrix.data[4]) + (position.y * viewmatrix.data[5]) + (position.z * viewmatrix.data[6]) + viewmatrix.data[7];
		quaternion.z = (position.x * viewmatrix.data[8]) + (position.y * viewmatrix.data[9]) + (position.z * viewmatrix.data[10]) + viewmatrix.data[11];
		quaternion.w = (position.x * viewmatrix.data[12]) + (position.y * viewmatrix.data[13]) + (position.z * viewmatrix.data[14]) + viewmatrix.data[15];

		if (quaternion.w < 0.1f)
			return Vector2::NaN();

		Vector3 ndc;
		ndc.x = quaternion.x / quaternion.w;
		ndc.y = quaternion.y / quaternion.w;
		ndc.z = quaternion.z / quaternion.w;

		return {
			(dimensions.x / 2 * ndc.x) + (ndc.x + dimensions.x / 2),
			-(dimensions.y / 2 * ndc.y) + (ndc.y + dimensions.y / 2)
		};
    }

    CFrame LookAt(Vector3 camera, Vector3 target) {
        Vector3 forward = Vector3{
            target.x - camera.x,
            target.y - camera.y,
            target.z - camera.z
        }.Normalize();

        Vector3 right = (Vector3{0, 1, 0}.Cross(forward)).Normalize();
        Vector3 up = forward.Cross(right);

        Matrix3x3 lookat{};

        lookat.data[0] = -right.x;
        lookat.data[1] = up.x;
        lookat.data[2] = -forward.x;

        lookat.data[3] = right.y;
        lookat.data[4] = up.y;
        lookat.data[5] = -forward.y;

        lookat.data[6] = -right.z;
        lookat.data[7] = up.z;
        lookat.data[8] = -forward.z;

        return CFrame{
            lookat,
            camera
        };
    }
}