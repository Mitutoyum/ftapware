#pragma once

#include <string>
#include <memory>
#include <expected>
#include <unordered_map>

#include "context.h"
#include "module_manager.h"
#include "modules/tabglitch.h"

#include "client/client.h"

namespace FTAPWare {
	class Client;

	class Engine {
	private:
		std::unique_ptr<Client> client { nullptr };
		ModuleManager module_manager;
		Context context;
	public:
		Engine();
		~Engine();

		std::expected<void, std::string> Attach(uint32_t PID);
		void Detach();
		bool IsAttached() const;

		void SetAutoGrabEnabled(bool enabled) const;
		bool IsAutoGrabEnabled() const;
		void LockAutoGrabTarget() const;
		std::string GetAutoGrabTarget() const;

		void SetPalletControlEnabled(bool enabled) const;
		bool IsPalletControlEnabled() const;

		void SetFOV(float FOV) const;
		float GetFOV() const;

		void TriggerTabGlitch() const;
		void SetTabGlitchMode(Modules::TabGlitch::Mode mode) const;
		Modules::TabGlitch::Mode GetTabGlitchMode() const;
	};
}