#pragma once

#include "module.h"

#include <mutex>
#include <string>
#include <thread>
#include <atomic>
#include <condition_variable>

#include "ftapware/engine/client/memory/roblox/player.h"

// class AutoGrab : public Module {
// private:
// 	std::mutex _mutex;
// 	std::condition_variable _cv_detect;
// 	std::condition_variable _cv_aim;
// 	std::condition_variable _cv_grab;
// 	std::condition_variable _cv_delay;


// 	Player current_target{};
// 	bool _on_delay{ false };
// 	bool _ready{ false };
// 	bool _grab_ready{ false };
// 	bool _grab_in_progress{ false };



// public:
// 	void Main();
// 	void SetupKeybinds();

// 	void Initialize() override;
// };

// namespace FTAPWare::Modules {
// 	class AutoGrab : public Module {
// 	private:
// 		std::atomic<bool> running { false };
// 		std::atomic<bool> enabled { false };
// 		bool fov_check { false };
// 		bool wall_check { false };

// 		std::thread worker;
// 		std::string target_name;
// 	private:
// 		void WorkerLoop();
// 	public:
// 		using Module::Module;
		
// 		void Start();
// 		void Stop();
	
// 		std::string GetName() const override;
// 		void SetEnabled(bool enabled);
// 		bool IsEnabled() const;

// 		void LockTarget();
// 		std::string GetTarget() const;
// 	};
// }

// namespace FTAPWare::Modules {

// 	class AutoGrab : public Module {
// 	private:
// 		std::atomic<bool> running { false };
// 		std::atomic<bool> enabled { false };

// 		bool fov_check { false };
// 		bool wall_check { false };


// 		bool ready { false };
// 		bool grab_ready { false };
// 		bool grab_in_progress{ false };

// 		std::mutex mutex;
// 		std::thread detection_worker;
// 		std::thread aim_worker;
// 		std::thread grab_worker;

// 		std::condition_variable detect_cv;
// 		std::condition_variable aim_cv;
// 		std::condition_variable grab_cv;
// 		std::condition_variable delay_cv;

// 		Roblox::Player current_target;
// 		std::string target_name;
// 	private:
// 		void DetectionLoop();
// 		void AimLoop();
// 		void GrabLoop();
// 	public:
// 		using Module::Module;
		
// 		void Start();
// 		void Stop();
	
// 		std::string GetName() const override;
// 		void SetEnabled(bool enabled);
// 		bool IsEnabled() const;

// 		void LockTarget();
// 		std::string GetTarget() const;
// 	};
// }	

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