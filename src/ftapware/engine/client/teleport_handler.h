#pragma once

#include <mutex>
#include <string>
#include <atomic>
#include <Windows.h>
#include <functional>
#include <unordered_map>

namespace FTAPWare {
	class Process;
	
	class TeleportHandler {
		public:
			enum class EventType { Join, Leave, Any };

			using Event = std::function<void()>;
		private:
			const Process& process;

			std::atomic<bool> is_running{ false };
			std::atomic<bool> is_loaded{ false };

			std::atomic<int> join_epoch{ 0 };
			std::atomic<int> leave_epoch{ 0 };

			std::unordered_map<EventType, std::vector<std::pair<std::string, Event>>> events;
			std::condition_variable event_cv;
			std::mutex event_mutex;

			std::thread worker;
		private:
			void Worker();
		public:
			TeleportHandler(const Process& process);
			~TeleportHandler();

			void Start();
			void Stop();

			int LeaveEpoch() const;
			int JoinEpoch() const;

			bool IsLoaded() const;
			bool IsRunning() const;

			void AddEvent(const std::string& name, EventType type, Event callback);
			void RemoveEvent(const std::string& name);
			void ClearEvents();
	};
}