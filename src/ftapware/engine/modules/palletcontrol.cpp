#include "palletcontrol.h"

#include "ftapware/engine/context.h"
#include "ftapware/engine/client/client.h"

#include "ftapware/engine/client/memory/roblox/basepart.h"
#include "ftapware/engine/client/memory/roblox/humanoid.h"
#include "ftapware/engine/client/memory/roblox/primitive.h"
#include "ftapware/engine/client/memory/roblox/weldconstraint.h"


namespace FTAPWare::Modules {
	std::string PalletControl::GetName() const {
		return "PalletControl";
	}

	void PalletControl::Start() {
		if (running.exchange(true)) return;

		detection_worker = std::thread(&PalletControl::DetectionLoop, this);
		velocity_worker = std::thread(&PalletControl::VelocityLoop, this);
	}

	void PalletControl::Stop() {
		running.store(false, std::memory_order_release);

		target_cv.notify_all();

		if (detection_worker.joinable())
			detection_worker.join();

		if (velocity_worker.joinable())
			velocity_worker.join();
	}

	void PalletControl::SetEnabled(bool enabled) {
		this->enabled.store(enabled, std::memory_order_release);
	}

	bool PalletControl::IsEnabled() const {
		return enabled.load(std::memory_order_acquire);
	}

	void PalletControl::DetectionLoop() {
		while (running.load(std::memory_order_acquire)) {
			if (!enabled.load(std::memory_order_acquire)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
				continue;
			}

			auto client = context.GetClient();

			if (!client) {
				{
					std::lock_guard lock(target_mutex);

					target.reset();
				}
				std::this_thread::sleep_for(std::chrono::seconds(1));
				continue;
			}

			auto game_session = client->GetGameSession();

			if (!game_session) {
				{
					std::lock_guard lock(target_mutex);

					target.reset();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
				continue;
			}

			auto humanoid = *(game_session->GetCaches().GetHumanoid().Get());

			if (!humanoid.IsValid()) {
				{
					std::lock_guard lock(target_mutex);

					target.reset();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}

			auto workspace = game_session->GetWorkspace();

			auto grabparts = workspace.FindFirstChild("GrabParts");

			if (!grabparts.IsValid()) {
				{
					std::lock_guard lock(target_mutex);

					target.reset();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}

			Roblox::WeldConstraint weldconstraint = grabparts.FindFirstChild("GrabPart").FindFirstChild("WeldConstraint");

			if (!weldconstraint.IsValid()) {
				{
					std::lock_guard lock(target_mutex);

					target.reset();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}

			Roblox::BasePart part1 = weldconstraint.Part1();

			if (!part1.IsValid()) {
				{
					std::lock_guard lock(target_mutex);

					target.reset();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}

			auto primitive = part1.GetPrimitive();

			if (primitive.GetSize() != PALLET_SIZE) {
				{
					std::lock_guard lock(target_mutex);

					target.reset();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}

			if (humanoid.GetFloorMaterial() != PALLET_MATERIAL) {
				{
					std::lock_guard lock(target_mutex);

					target.reset();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}

			bool notify = false;

			{
				std::lock_guard lock(target_mutex);

				if (!target.has_value()) {
					target = primitive;
					notify = true;
				}
			}
			
			if (notify)
				target_cv.notify_one();
		}
	}

	void PalletControl::VelocityLoop() {
		while (running.load(std::memory_order_acquire)) {
			std::unique_lock lock(target_mutex);

			target_cv.wait(lock, [this]() {
				return !running.load(std::memory_order_acquire) || target.has_value();
			});

			if (!running.load(std::memory_order_acquire)) {
				break;
			}

			Roblox::Primitive primitive = target.value();
			lock.unlock();

			while (running.load(std::memory_order_acquire) && enabled.load(std::memory_order_acquire)) {
				{
					std::lock_guard lock(target_mutex);

					if (!target) {
						break;
					}
				}

				auto client = context.GetClient();
				if (!client)
					break;

				auto game_session = client->GetGameSession();
				if (!game_session) 
					break;

				auto humanoidrootpart = game_session->GetCaches().GetHumanoidRootPart().Get();
				if (!humanoidrootpart || !humanoidrootpart->IsValid())
					break;

				Math::Vector3 velocity = primitive.GetAssemblyLinearVelocity();
				humanoidrootpart->GetPrimitive().SetAssemblyLinearVelocity(velocity);
			}
		}
	}
}