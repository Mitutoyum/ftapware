#include "teleport_handler.h"

#include "memory/roblox/datamodel.h"
#include "process.h"

#include <Windows.h>
#include <vector>
#include <thread>
#include <mutex>

namespace FTAPWare {
	TeleportHandler::TeleportHandler(const Process& process) : process(process) {};

	TeleportHandler::~TeleportHandler() {
		Stop();
	}

	int TeleportHandler::JoinEpoch() const {
		return join_epoch.load(std::memory_order_acquire);
	}

	int TeleportHandler::LeaveEpoch() const {
		return leave_epoch.load(std::memory_order_acquire);
	}

	bool TeleportHandler::IsRunning() const {
		return is_running.load(std::memory_order_acquire);
	}

	bool TeleportHandler::IsLoaded() const {
		return is_loaded.load(std::memory_order_acquire);
	}

	void TeleportHandler::ClearEvents() {
		std::lock_guard lock(event_mutex);
		events.clear();
	}

	void TeleportHandler::AddEvent(const std::string& name, EventType type, Event callback) {
		std::lock_guard lock(event_mutex);

		auto add_or_replace = [&](const std::string& event_name, EventType event_type, Event event_callback) {
			auto& events_of_type = events[event_type];

			auto it = std::find_if(events_of_type.begin(), events_of_type.end(), [&](const std::pair<std::string, Event>& pair) {
				return pair.first == event_name;
			});

			if (it != events_of_type.end()) {
				it->second = std::move(event_callback);
			}
			else {
				events_of_type.push_back({ event_name, event_callback });
			}
		};

		if (type == EventType::Any) {
			add_or_replace(name, EventType::Join, callback);
			add_or_replace(name, EventType::Leave, callback);
		}
		else {
			add_or_replace(name, type, callback);
		}
	}

	void TeleportHandler::RemoveEvent(const std::string& name) {
		std::lock_guard<std::mutex> lock(event_mutex);

		for (auto& [type, events_of_type] : events) {
			events_of_type.erase(std::remove_if(events_of_type.begin(), events_of_type.end(), [&](const std::pair<std::string, Event>& pair) {
				return pair.first == name;
			}), events_of_type.end());
		};
	}

	void TeleportHandler::Worker() {
		while (is_running.load(std::memory_order_acquire)) {
			auto datamodel = Roblox::DataModel::Get(process);

			while (is_running.load(std::memory_order_acquire)) {
				if (datamodel.IsGameLoaded()) {
					break;
				}

				datamodel = Roblox::DataModel::Get(process);
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}

			if (!is_running.load())
				break;

			{
				std::vector<std::pair<std::string, Event>> events_by_type;

				{
					std::lock_guard<std::mutex> lock(event_mutex);
					events_by_type = events[EventType::Join];
				}

				for (const auto& [name, event] : events_by_type) {
					if (!is_running.load())
						break;
					event();
				}
			}

			join_epoch.fetch_add(1, std::memory_order_acq_rel);
			is_loaded.store(true, std::memory_order_release);
			event_cv.notify_all();

			while (is_running.load()) {
				if (!datamodel.IsGameLoaded())
					break;

				datamodel = Roblox::DataModel::Get(process);
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}

			if (!is_running.load())
				break;

			leave_epoch.fetch_add(1, std::memory_order_acq_rel);
			is_loaded.store(false, std::memory_order_release);
			event_cv.notify_all();

			{
				std::vector<std::pair<std::string, Event>> events_by_type;

				{
					std::lock_guard<std::mutex> lock(event_mutex);
					events_by_type = events[EventType::Leave];
				}

				for (const auto& [name, event] : events_by_type) {
					if (!is_running.load())
						break;
					event();
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(150));
		}
	}

	void TeleportHandler::Start() {
		if (is_running.exchange(true)) {
			return;
		}

		worker = std::thread(&TeleportHandler::Worker, this);
	}

	void TeleportHandler::Stop() {
		if (!is_running.exchange(false)) {
			return;
		}

		event_cv.notify_all();

		if (worker.joinable() && std::this_thread::get_id() != worker.get_id()) {
			worker.join();
		}
	}
}