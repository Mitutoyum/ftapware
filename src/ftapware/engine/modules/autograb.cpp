#include "autograb.h"

#include "ftapware/common/win32.h"

#include "ftapware/common/math/utils.h"
#include "ftapware/common/math/cframe.h"
#include "ftapware/common/math/vector2.h"
#include "ftapware/common/math/vector3.h"
#include "ftapware/common/math/matrix4x4.h"

#include "ftapware/engine/context.h"
#include "ftapware/engine/client/client.h"
#include "ftapware/engine/client/common/player.h"

#include "ftapware/engine/client/memory/roblox/player.h"
#include "ftapware/engine/client/memory/roblox/basepart.h"
#include "ftapware/engine/client/memory/roblox/primitive.h"
#include "ftapware/engine/client/memory/roblox/modelinstance.h"

#include <iostream>
#include <mutex>
#include <algorithm>

namespace FTAPWare::Modules {
	void AutoGrab::Start() {
		if (running.exchange(true)) return;

		detection_worker = std::thread(&AutoGrab::DetectionLoop, this);
		grab_worker = std::thread(&AutoGrab::GrabLoop, this);
	}

	void AutoGrab::Stop() {
		running.store(false, std::memory_order_release);

		if (detection_worker.joinable())
			detection_worker.join();

		if (grab_worker.joinable())
			grab_worker.join();
	}

	std::string AutoGrab::GetName() const {
		return "AutoGrab";
	}

	void AutoGrab::SetEnabled(bool enabled) {
		this->enabled.store(enabled, std::memory_order_release);
	}

	bool AutoGrab::IsEnabled() const {
		return enabled.load(std::memory_order_acquire);
	}

	bool AutoGrab::LockTarget() {
		if (!target_name.empty()) {
			target_name = "";
			return true;
		}

		auto client = context.GetClient();

		if (!client) return true;

		auto game_session = client->GetGameSession();

		if (!game_session) return true;

		auto playerlist = game_session->GetCaches().GetPlayerList().Get();

		if (!playerlist) return true;

		auto player = GetClosestPlayerToMouse(
			*playerlist,
			game_session->GetLocalPlayer(),
			game_session->GetVisualEngine().GetViewMatrix(),
			game_session->GetVisualEngine().GetDimensions(),
			game_session->GetInputObject().GetPosition()
		);

		if (!player.IsValid()) return false;

		target_name = player.Name();
		return true;
	}

	std::string AutoGrab::GetTarget() const {
		return target_name;
	}

	void AutoGrab::DetectionLoop() {
		while (running.load(std::memory_order_acquire)) {
			if (!enabled.load(std::memory_order_acquire)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				continue;
			}

			auto client = context.GetClient();
			if (!client) {
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
				continue;
			}

			auto game_session = client->GetGameSession();
			if (!game_session) {
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
				continue;
			}

			if (!target_name.empty()) {
				Roblox::Player target = game_session->GetPlayers().FindFirstChild(target_name);

				if (!target.IsValid()) {
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					continue;
				}

				while (running.load(std::memory_order_acquire) && enabled.load(std::memory_order_acquire)) {
					auto local_humanoidrootpart = game_session->GetCaches().GetHumanoidRootPart().Get();
					if (!local_humanoidrootpart || !local_humanoidrootpart->IsValid()) break;

					auto target_character = target.GetCharacter();
					Roblox::BasePart target_humanoidrootpart = target_character.FindFirstChild("HumanoidRootPart");
					if (!target_humanoidrootpart.IsValid())
						break;

					auto target_position = target_humanoidrootpart.GetPrimitive().GetPosition();
					auto local_position = local_humanoidrootpart->GetPrimitive().GetPosition();

					float magnitude = (local_position - target_position).Magnitude();
					if (magnitude > 50)
						break;

					auto camera = game_session->GetCamera();
					Math::Vector3 camera_position = camera.GetPosition();
					Math::Matrix3x3 camera_rotation = Math::LookAt(camera_position, target_position).rotation;
					camera.SetRotation(camera_rotation);

					auto speed = local_humanoidrootpart->GetPrimitive().GetAssemblyLinearVelocity().Magnitude();
					auto grab_distance = 30;

					if (speed >= 200) {
						grab_distance = std::clamp(30.0 - (speed * 0.5), 20.0, 30.0);
					}

					if (magnitude < grab_distance) {
						std::lock_guard grab_lock(mutex);

						if (!grab_ready && !grab_in_progress) {
							grab_ready = true;
							cv.notify_one();
						}
					}

					{
						std::unique_lock grab_lock(mutex);
						if (grab_in_progress) {
							cv.wait(grab_lock, [&]() { return !grab_in_progress; });
						}
					}
				}
			}
		}
	}

	void AutoGrab::GrabLoop() {
		while (running.load(std::memory_order_acquire)) {
			std::unique_lock lock(mutex);

			cv.wait(lock, [this] {
				return !running.load(std::memory_order_acquire) || grab_ready;
			});

			if (!running.load(std::memory_order_acquire))
				break;

			auto game_session = context.GetClient()->GetGameSession();

			lock.unlock();

			Win32::LeftClick();

			while (!IsGrabbing(game_session->GetWorkspace())) {
				Win32::LeftClick();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}

			lock.lock();
			grab_in_progress = true;
			lock.unlock();

			std::this_thread::sleep_for(std::chrono::seconds(2));

			lock.lock();
			grab_in_progress = false;
			grab_ready = false;
			lock.unlock();

			cv.notify_all();
		}
	}
}