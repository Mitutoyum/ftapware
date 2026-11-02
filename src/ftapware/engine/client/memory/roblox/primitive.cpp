#include "primitive.h"

#include "ftapware/common/math/vector3.h"
#include "ftapware/common/math/matrix3x3.h"
#include "ftapware/engine/client/memory/offsets.h"

namespace FTAPWare::Roblox {
	Math::Vector3 Primitive::GetSize() const {
		Math::Vector3 size = ReadFrom<Math::Vector3>(Offsets::Primitive::Size);

		return size;
	}

	Math::Vector3 Primitive::GetPosition() const {
		Math::Vector3 position = ReadFrom<Math::Vector3>(Offsets::Primitive::Position);
		
		return position;
	}

	Math::Matrix3x3 Primitive::GetRotation() const {
		Math::Matrix3x3 rotation = ReadFrom<Math::Matrix3x3>(Offsets::Primitive::Rotation);

		return rotation;
	}

	void Primitive::SetPosition(const Math::Vector3& position) const {
		WriteTo<Math::Vector3>(Offsets::Primitive::Position, position);
	}

	void Primitive::SetRotation(const Math::Matrix3x3& rotation) const {
		WriteTo<Math::Matrix3x3>(Offsets::Primitive::Rotation, rotation);
	}

	Math::Vector3 Primitive::GetAssemblyAngularVelocity() const {
		Math::Vector3 velocity = ReadFrom<Math::Vector3>(Offsets::Primitive::AssemblyAngularVelocity);

		return velocity;
	}

	Math::Vector3 Primitive::GetAssemblyLinearVelocity() const {
		Math::Vector3 velocity = ReadFrom<Math::Vector3>(Offsets::Primitive::AssemblyLinearVelocity);

		return velocity;
	}

	void Primitive::SetAssemblyAngularVelocity(const Math::Vector3& velocity) const {
		WriteTo<Math::Vector3>(Offsets::Primitive::AssemblyAngularVelocity, velocity);
	}

	void Primitive::SetAssemblyLinearVelocity(const Math::Vector3& velocity) const {
		WriteTo<Math::Vector3>(Offsets::Primitive::AssemblyLinearVelocity, velocity);
	}
}