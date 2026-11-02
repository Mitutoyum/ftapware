#pragma once

#include "ftapware/engine/client/memory/remote_object.h"

namespace FTAPWare::Math {
	struct Vector3;
	struct Matrix3x3;
}

namespace FTAPWare::Roblox {
	class Primitive : public RemoteObject {
	public:
		using RemoteObject::RemoteObject;

		Math::Vector3 GetSize() const;
		Math::Vector3 GetPosition() const;
		Math::Matrix3x3 GetRotation() const;
		void SetPosition(const Math::Vector3& position) const;
		void SetRotation(const Math::Matrix3x3& rotation) const;

		Math::Vector3 GetAssemblyLinearVelocity() const;
		Math::Vector3 GetAssemblyAngularVelocity() const;
		void SetAssemblyLinearVelocity(const Math::Vector3& velocity) const;
		void SetAssemblyAngularVelocity(const Math::Vector3& velocity) const;
	};
}