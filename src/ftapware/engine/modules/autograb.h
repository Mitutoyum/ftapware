#pragma once

#include "module.h"

#include <mutex>
#include <string>
#include <thread>
#include <atomic>
#include <condition_variable>

#include "ftapware/engine/client/memory/roblox/player.h"

namespace FTAPWare::Modules {
	class AutoGrab : public Module {
	private:
		std::atomic<bool> running { false };
		std::atomic<bool> enabled { false };

		bool fov_check { false };
		bool wall_check { false };

		bool grab_ready { false };
		bool grab_in_progress{ false };

		std::condition_variable cv;
		std::mutex mutex;

		std::thread detection_worker;
		std::thread grab_worker;

		Roblox::Player current_target;
		std::string target_name;
	private:
		void DetectionLoop();
		void GrabLoop();
	public:
		using Module::Module;
		
		void Start();
		void Stop();
	
		std::string GetName() const override;
		void SetEnabled(bool enabled);
		bool IsEnabled() const;

		bool LockTarget();
		std::string GetTarget() const;
	};
}