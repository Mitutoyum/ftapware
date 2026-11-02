#pragma once

#include <mutex>
#include <dxgi.h>
#include <d3d11.h>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "keybind_manager.h"
#include "logstreambuf.h"

namespace FTAPWare {
	class Engine;

	enum class LogLevel {
		Info,
		Warning,
		Error
	};

	struct LogEntry {
		std::string message;
		LogLevel level;
	};

	class GUI {
	private:
		bool vsync = true;
		bool running = false;
		bool auto_scroll = true;	

		Engine& engine;
		KeybindManager keybind_manager;

		std::mutex log_mutex;
		std::vector<LogEntry> logs;

		LogStreamBuf cout_buffer;
		LogStreamBuf cerr_buffer;

		std::streambuf* original_cout = nullptr;
		std::streambuf* original_cerr = nullptr;

		ID3D11Device* device = nullptr;
		IDXGISwapChain* swapchain = nullptr;
		ID3D11DeviceContext* device_context = nullptr;
		ID3D11RenderTargetView* render_target = nullptr;


		std::vector<uint32_t> process_ids;
		int selected_process = -1;

		static constexpr const char* tabglitch_modes[] = {
			"Velocity",
			"Position"
		};

		bool keybind_listener_enabled = true;

	private:
		bool CreateDevice(HWND window);
		bool CreateRenderTarget();

		void CleanupDevice();
		void CleanupRenderTarget();

		static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void Draw();
	public:
		GUI(Engine& engine);
		~GUI();

		void Run();
		void Stop();

		void ResizeWindow(int width, int height);
	};
}