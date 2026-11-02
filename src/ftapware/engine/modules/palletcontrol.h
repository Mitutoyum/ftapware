#pragma once

#include <string>
#include <atomic>
#include <thread>
#include <optional>
#include <condition_variable>

#include "module.h"
#include "ftapware/common/math/vector3.h"
#include "ftapware/engine/client/memory/roblox/primitive.h"

// namespace FTAPWare::Roblox {
// 	class Primitive;
// }

namespace FTAPWare::Modules {
	class PalletControl : public Module {
	private:
		static constexpr Math::Vector3 PALLET_SIZE {
			11.5f,
			1.5f,
			11.5f,
		};

		static constexpr int PALLET_MATERIAL = 528;
	private:
		std::atomic<bool> running { false };
		std::atomic<bool> enabled { false };
		
		mutable std::mutex target_mutex;
		std::condition_variable target_cv;
		std::optional<Roblox::Primitive> target;

		std::thread detection_worker;
		std::thread velocity_worker;

	private:
		void DetectionLoop();
		void VelocityLoop();
	public:
		using Module::Module;

		void Start();
		void Stop();
		
		std::string GetName() const override;
		void SetEnabled(bool enabled);
		bool IsEnabled() const;
	};
}