#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "ftapware/engine/client/memory/remote_object.h"

namespace FTAPWare::Roblox {
	class Instance : public RemoteObject {
	public:
		using RemoteObject::RemoteObject;

		Instance Parent() const;
		std::string Name() const;
		std::string ClassName() const;

		std::vector<Instance> GetChildren() const;
		std::vector<Instance> GetDescendants() const;

		Instance FindFirstChild(const std::string_view name) const;
		Instance FindFirstDescendant(const std::string_view name) const;

		Instance FindFirstChildOfClass(const std::string_view classname) const;
		Instance FindFirstChildWhichIsA(const std::string_view classname) const;

		bool IsA(const std::string_view classname) const;
	};
}