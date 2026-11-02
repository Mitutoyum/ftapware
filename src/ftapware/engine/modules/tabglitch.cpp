#include "tabglitch.h"

#include "ftapware/common/math/vector3.h"

#include "ftapware/engine/context.h"
#include "ftapware/engine/client/client.h"

#include "ftapware/engine/client/memory/roblox/basepart.h"
#include "ftapware/engine/client/memory/roblox/primitive.h"

namespace FTAPWare::Modules {
	std::string TabGlitch::GetName() const {
		return "tabglitch";
	}

	void TabGlitch::SetMode(Mode mode) {
		this->mode = mode;
	}

	TabGlitch::Mode TabGlitch::GetMode() {
		return mode;
	}

	void TabGlitch::Trigger() const {
		auto client = context.GetClient();

		if (!client) {
			return;
		}

		auto game_session = client->GetGameSession();
		
		if (!game_session) {
			return;
		}

		auto humanoidrootpart = game_session->GetCaches().GetHumanoidRootPart().Get();

		if (!humanoidrootpart || !humanoidrootpart->IsValid()) {
			return;
		}

		auto primitive = humanoidrootpart->GetPrimitive();

		if (mode == TabGlitch::Mode::Velocity) {
			Math::Vector3 velocity = { 0, 0, 0 };

			for (int i = 0; i < 10000; i++) {
				primitive.SetAssemblyLinearVelocity(velocity);
			}
		} else {
			Math::Vector3 position = primitive.GetPosition();

			if (!position.IsValid())
				return;

			for (int i = 0; i < 10000; i++) {
				primitive.SetPosition(position);
			}
		}
	}
}