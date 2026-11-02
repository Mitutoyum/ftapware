#include "player.h"
#include "modelinstance.h"
#include "ftapware/engine/client/memory/offsets.h"

#include <iostream>

namespace FTAPWare::Roblox {
	Player::Player(const Instance& instance) : Instance(instance) {}

	ModelInstance Player::GetCharacter() const {
		uintptr_t modelinstance = ReadFrom<uintptr_t>(Offsets::Player::ModelInstance);

		if (!modelinstance)
			return ModelInstance();

		return ModelInstance(modelinstance, process);
	}
}