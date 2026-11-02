#include "engine.h"
#include "client/client.h"

#include "modules/fov.h"
#include "modules/autograb.h"
#include "modules/tabglitch.h"
#include "modules/palletcontrol.h"

#include "ftapware/common/win32.h"

#include <Windows.h>
#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <expected>

namespace FTAPWare {
	Engine::Engine() {
		module_manager.AddModule(std::make_unique<Modules::FOV>(context));
		module_manager.AddModule(std::make_unique<Modules::AutoGrab>(context));
		module_manager.AddModule(std::make_unique<Modules::TabGlitch>(context));
		module_manager.AddModule(std::make_unique<Modules::PalletControl>(context));


		auto autograb = module_manager.GetModule<Modules::AutoGrab>();
		auto palletcontrol = module_manager.GetModule<Modules::PalletControl>();

		autograb->Start();
		palletcontrol->Start();
	}

	Engine::~Engine() {
		auto autograb = module_manager.GetModule<Modules::AutoGrab>();
		auto palletcontrol = module_manager.GetModule<Modules::PalletControl>();
 
		autograb->Stop();
		palletcontrol->Stop();
	}
	
	std::expected<void, std::string> Engine::Attach(uint32_t PID) {
		if (this->client.get()) {
			std::cout << "Engine was already attached to process " << PID << '\n';
			return std::unexpected("Engine is already attached");
		}

		if (!Win32::IsProcessNamed(PID, L"RobloxPlayerBeta.exe")) {
			return std::unexpected("Process is not Roblox");
		}

		try {
			auto client = std::make_unique<Client>(PID);
			this->client = std::move(client);
			this->context.SetClient(this->client.get());
		} catch (const std::exception& e) {
			return std::unexpected(e.what());
		}

		std::cout << "Attached to process " << PID << '\n';

		return {};
	}

	void Engine::Detach() {
		if (this->client) {
			this->context.SetClient(nullptr);
			this->client.reset();
		}
	}

	bool Engine::IsAttached() const {
		if (client)
			return true;
		return false;
	}

	void Engine::SetAutoGrabEnabled(bool enabled) const {
		auto autograb = module_manager.GetModule<Modules::AutoGrab>();

		autograb->SetEnabled(enabled);

		if (enabled) {
			std::cout << "Enabled AutoGrab\n";
		} else {
			std::cout << "Disabled AutoGrab\n";
		}
	}

	bool Engine::IsAutoGrabEnabled() const {
		auto autograb = module_manager.GetModule<Modules::AutoGrab>();

		return autograb->IsEnabled();
	}

	void Engine::LockAutoGrabTarget() const {
		auto autograb = module_manager.GetModule<Modules::AutoGrab>();

		bool result = autograb->LockTarget();

		if (!result) {
			std::cout << "No target found\n";
		} else {
			std::string target = autograb->GetTarget();

			if (target.empty()) {
				std::cout << "Unlocked target\n";
			} else {
				std::cout << "Locked target: " << target << "\n";
			}
		}
	}

	std::string Engine::GetAutoGrabTarget() const {
		auto autograb = module_manager.GetModule<Modules::AutoGrab>();
		
		return autograb->GetTarget();
	}

	void Engine::SetPalletControlEnabled(bool enabled) const {
		auto palletcontrol = module_manager.GetModule<Modules::PalletControl>();

		palletcontrol->SetEnabled(enabled);

		if (enabled) {
			std::cout << "Enabled PalletControl\n";
		} else {
			std::cout << "Disabled PalletControl\n";
		}
	}

	bool Engine::IsPalletControlEnabled() const {
		auto palletcontrol = module_manager.GetModule<Modules::PalletControl>();

		return palletcontrol->IsEnabled();
	}

	void Engine::SetFOV(float FOV) const {
		auto fov = module_manager.GetModule<Modules::FOV>();

		fov->SetFOV(FOV);

		std::cout << "Set FOV to " << FOV << '\n';
	}

	float Engine::GetFOV() const {
		auto tabglitch = module_manager.GetModule<Modules::FOV>();

		return tabglitch->GetFOV();
	}

	void Engine::TriggerTabGlitch() const {
		auto tabglitch = module_manager.GetModule<Modules::TabGlitch>();

		tabglitch->Trigger();

		std::cout << "Triggered TabGlitch\n";
	}

	void Engine::SetTabGlitchMode(Modules::TabGlitch::Mode mode) const {
		auto tabglitch = module_manager.GetModule<Modules::TabGlitch>();

		tabglitch->SetMode(mode);

		std::cout << "Set TabGlitch mode to " << (mode == Modules::TabGlitch::Mode::Velocity ? "Velocity" : "Position") << '\n';
	}

	Modules::TabGlitch::Mode Engine::GetTabGlitchMode() const {
		auto tabglitch = module_manager.GetModule<Modules::TabGlitch>();

		return tabglitch->GetMode();
	}
}