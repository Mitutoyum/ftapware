#include "camera.h"

#include "ftapware/common/math/vector2.h"
#include "ftapware/common/math/vector3.h"
#include "ftapware/common/math/matrix3x3.h"
#include "ftapware/common/math/matrix4x4.h"
#include "ftapware/common/math/quaternion.h"
#include "ftapware/engine/client/memory/offsets.h"

#include <numbers>

namespace FTAPWare::Roblox {
	Camera::Camera(const Instance& instance) : Instance(instance) {}

	Math::Vector3 Camera::GetPosition() const {
		return ReadFrom<Math::Vector3>(Offsets::Camera::Position);
	}

	void Camera::SetPosition(Math::Vector3 position) const {
		WriteTo<Math::Vector3>(Offsets::Camera::Position, position);
	}

	Math::Matrix3x3 Camera::GetRotation() const {
		return ReadFrom<Math::Matrix3x3>(Offsets::Camera::Rotation);
	}

	void Camera::SetRotation(Math::Matrix3x3 rotation) const {
		WriteTo<Math::Matrix3x3>(Offsets::Camera::Rotation, rotation);
	}

	float Camera::GetFieldOfView() const {
		return ReadFrom<float>(Offsets::Camera::FieldOfView) * 180 / std::numbers::pi;
	}

	void Camera::SetFieldOfView(float value) const {
		WriteTo<float>(Offsets::Camera::FieldOfView, value * std::numbers::pi / 180);
	}
}