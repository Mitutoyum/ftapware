#include "modelinstance.h"

#include "basepart.h"
#include "humanoid.h"
#include "ftapware/engine/client/memory/offsets.h"

namespace FTAPWare::Roblox {
	ModelInstance::ModelInstance(const Instance& instance) : Instance(instance) {}

	BasePart ModelInstance::GetHumanoidRootPart() const {
		Instance humanoidrootpart = FindFirstChild("HumanoidRootPart");

		if (!humanoidrootpart.IsValid())
			return BasePart();

		return BasePart(humanoidrootpart);
	}

	Humanoid ModelInstance::GetHumanoid() const {
		Instance humanoid = FindFirstChildOfClass("Humanoid");

		if (!humanoid.IsValid())
			return Humanoid();

		return Humanoid(humanoid);
	}
}