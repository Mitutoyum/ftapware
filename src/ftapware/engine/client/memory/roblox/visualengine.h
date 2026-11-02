#pragma once

#include "ftapware/engine/client/memory/remote_object.h"

namespace FTAPWare::Math {
	struct Vector2;
	struct Matrix4x4;
}

namespace FTAPWare::Roblox {
	class VisualEngine : public RemoteObject {
	public:
		using RemoteObject::RemoteObject;

		static VisualEngine Get(const Process& process);

		Math::Vector2 GetDimensions() const;
		Math::Matrix4x4 GetViewMatrix() const;
	};
}