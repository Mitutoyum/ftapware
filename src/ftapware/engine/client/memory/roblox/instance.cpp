#include "instance.h"

#include "ftapware/engine/client/process.h"
#include "ftapware/engine/client/memory/offsets.h"

#include <vector>
#include <string>
#include <ranges>

namespace FTAPWare::Roblox {
	std::string Instance::Name() const {
		uintptr_t name_container = ReadFrom<uintptr_t>(Offsets::Instance::NameContainer);
		uintptr_t name_ptr = name_container + Offsets::Instance::Name;
		std::string name = "";

		try {
			name = process->Read<std::string>(name_ptr);
		}
		catch (...) {}

		return name;
	}

	std::string Instance::ClassName() const {
		uintptr_t classdescriptor = ReadFrom<uintptr_t>(Offsets::Instance::ClassDescriptor);
		uintptr_t classname_ptr = process->Read<uintptr_t>(classdescriptor + Offsets::Instance::ClassName);

		if (!classname_ptr) {
			return "";
		}

		std::string classname = "";

		try {
			classname = process->Read<std::string>(classname_ptr);
		}
		catch (...) {}

		return classname;
	}

	Instance Instance::Parent() const {
		uintptr_t parent = ReadFrom<uintptr_t>(Offsets::Instance::Parent);
		return Instance(parent, process);
	}

	std::vector<Instance> Instance::GetChildren() const {
		std::vector<Instance> children;

		uintptr_t children_ptr = ReadFrom<uintptr_t>(Offsets::Instance::ChildrenStart);

		if (!children_ptr)
			return children;

		uintptr_t children_start = process->Read<uintptr_t>(children_ptr);
		uintptr_t children_end = process->Read<uintptr_t>(children_ptr + Offsets::Instance::ChildrenEnd);

		if (!children_start || children_start == children_end)
			return children;

		for (; children_start < children_end; children_start += 0x10) {
			uintptr_t child = process->Read<uintptr_t>(children_start);
			if (child) {
				children.emplace_back(child, process);
			}
		}

		return children;
	}

	Instance Instance::FindFirstChild(const std::string_view name) const {
		if (!IsValid())
			return Instance();

		for (const auto& child : GetChildren()) {
			if (child.Name() == name)
				return child;

		}

		return Instance();
	}

	Instance Instance::FindFirstDescendant(const std::string_view name) const {
		if (!IsValid())
			return Instance();

		for (const auto& child : GetDescendants()) {
			if (child.Name() == name)
				return child;
		}

		return Instance();
	}

	Instance Instance::FindFirstChildOfClass(const std::string_view classname) const {
		if (!IsValid())
			return Instance();

		for (const auto& child : GetChildren()) {
			if (child.ClassName() == classname)
				return child;
		}

		return Instance();
	}

	Instance Instance::FindFirstChildWhichIsA(const std::string_view classname) const {
		if (!IsValid())
			return Instance();

		for (const auto& child : GetChildren()) {
			if (child.IsA(classname))
				return child;
		}

		return Instance();
	}

	bool Instance::IsA(const std::string_view classname) const {
		uintptr_t classdescriptor = ReadFrom<uintptr_t>(Offsets::Instance::ClassDescriptor);

		while (true) {
			if (!classdescriptor) break;

			uintptr_t base_classname_ptr = process->Read<uintptr_t>(classdescriptor + Offsets::Instance::ClassName);

			if (!base_classname_ptr) break;

			try {
				std::string base_classname = process->Read<std::string>(base_classname_ptr);

				if (base_classname == classname)
					return true;

				if (base_classname == "<<<ROOT>>>")
					return false;
			}
			catch (...) {}

			classdescriptor = process->Read<uintptr_t>(classdescriptor + Offsets::Instance::ClassBase);
		}

		return false;
	}

	std::vector<Instance> Instance::GetDescendants() const {
		std::vector<Instance> descendants;

		for (const auto& child : GetChildren()) {
			std::vector<Instance> child_descendants = child.GetDescendants();

			descendants.push_back(child);
			descendants.insert(descendants.end(),
				std::make_move_iterator(child_descendants.begin()),
				std::make_move_iterator(child_descendants.end()));
		}

		return descendants;
	}


	//Vector2 Instance::WorldToScreen(const Matrix4x4& viewmatrix, const Vector2& dimensions) const {
	//	Vector3 position = GetPosition();
	//
	//	Quaternion quaternion;
	//
	//	quaternion.x = (position.x * viewmatrix.data[0]) + (position.y * viewmatrix.data[1]) + (position.z * viewmatrix.data[2]) + viewmatrix.data[3];
	//	quaternion.y = (position.x * viewmatrix.data[4]) + (position.y * viewmatrix.data[5]) + (position.z * viewmatrix.data[6]) + viewmatrix.data[7];
	//	quaternion.z = (position.x * viewmatrix.data[8]) + (position.y * viewmatrix.data[9]) + (position.z * viewmatrix.data[10]) + viewmatrix.data[11];
	//	quaternion.w = (position.x * viewmatrix.data[12]) + (position.y * viewmatrix.data[13]) + (position.z * viewmatrix.data[14]) + viewmatrix.data[15];
	//
	//	if (quaternion.w < 0.1f)
	//		return Vector2::NaN();
	//
	//	Vector3 ndc;
	//	ndc.x = quaternion.x / quaternion.w;
	//	ndc.y = quaternion.y / quaternion.w;
	//	ndc.z = quaternion.z / quaternion.w;
	//
	//	return {
	//		(dimensions.x / 2 * ndc.x) + (ndc.x + dimensions.x / 2),
	//		-(dimensions.y / 2 * ndc.y) + (ndc.y + dimensions.y / 2)
	//	};
	//}
}