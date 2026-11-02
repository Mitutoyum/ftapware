#pragma once

#include "instance.h"

namespace FTAPWare::Math {
	struct Vector2;
	struct Vector3;
	struct Matrix3x3;
	struct Matrix3x3;
}

namespace FTAPWare::Roblox {
	class Camera : public Instance {
	public:
		using Instance::Instance;
		Camera(const Instance& instance);

		Math::Vector3 GetPosition() const;
		void SetPosition(Math::Vector3 position) const;

		Math::Matrix3x3 GetRotation() const;
		void SetRotation(Math::Matrix3x3 rotation) const;

		float GetFieldOfView() const;
		void SetFieldOfView(float value) const;
	};
}