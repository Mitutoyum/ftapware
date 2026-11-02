#include "gui.h"

#include "common/draw.h"

#include "ftapware/common/win32.h"
#include "ftapware/engine/engine.h"

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <dwmapi.h>
#include <stdexcept>
#include <tchar.h>
#include <iostream>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace FTAPWare {
    GUI::GUI(Engine& engine) : engine(engine), cout_buffer([this](const std::string& message) {
        std::lock_guard lock(log_mutex);
        logs.push_back({ message, LogLevel::Info });
    }),
    cerr_buffer([this](const std::string& message) {
        std::lock_guard lock(log_mutex);
        logs.push_back({ message, LogLevel::Error });
    }) {
        original_cout = std::cout.rdbuf(&cout_buffer);
        original_cerr = std::cerr.rdbuf(&cerr_buffer);

        keybind_manager.RegisterKey(
            "autograb.toggle",
            0,
            [&engine]() {
                engine.SetAutoGrabEnabled(!engine.IsAutoGrabEnabled());
            }
        );

        keybind_manager.RegisterKey(
            "autograb.lock",
            0,
            [&engine]() {
                engine.LockAutoGrabTarget();
            }
        );

        keybind_manager.RegisterKey(
            "tabglitch.trigger",
            0,
            [&engine]() {
                engine.TriggerTabGlitch();
            }
        );

        keybind_manager.RegisterKey(
            "palletcontrol.toggle",
            0,
            [&engine]() {
                engine.SetPalletControlEnabled(!engine.IsPalletControlEnabled());
            }
        );
    }
    GUI::~GUI() {
        Stop();

        if (original_cout) std::cout.rdbuf(original_cout);
        if (original_cerr) std::cerr.rdbuf(original_cerr);
    }

    bool GUI::CreateDevice(HWND window) {
        DXGI_SWAP_CHAIN_DESC swapchain_desc;
        ZeroMemory(&swapchain_desc, sizeof(swapchain_desc));
        swapchain_desc.BufferCount = 2;
        swapchain_desc.BufferDesc.Width = 0;
        swapchain_desc.BufferDesc.Height = 0;
        swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
        swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
        swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapchain_desc.OutputWindow = window;
        swapchain_desc.SampleDesc.Count = 1;
        swapchain_desc.SampleDesc.Quality = 0;
        swapchain_desc.Windowed = TRUE;
        swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT flags = 0;
        D3D_FEATURE_LEVEL feature_level;
        const D3D_FEATURE_LEVEL feature_levels[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

        HRESULT result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, feature_levels, 2, D3D11_SDK_VERSION, &swapchain_desc, &swapchain, &device, &feature_level, &device_context);

        if (result == DXGI_ERROR_UNSUPPORTED)
            result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, flags, feature_levels, 2, D3D11_SDK_VERSION, &swapchain_desc, &swapchain, &device, &feature_level, &device_context);

        if (result != S_OK)
            return false;

        return true;
    }

    bool GUI::CreateRenderTarget() {
        ID3D11Texture2D* back_buffer;
        HRESULT result = swapchain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));

        if (SUCCEEDED(result) && back_buffer) {
            result = device->CreateRenderTargetView(back_buffer, nullptr, &render_target);
            back_buffer->Release();
            return SUCCEEDED(result);
        }

        return false;
    }

    void GUI::CleanupDevice() {
        if (swapchain) {
            swapchain->Release();
            swapchain = nullptr;
        }

        if (device_context) {
            device_context->Release();
            device_context = nullptr;
        }

        if (device) {
            device->Release();
            device = nullptr;
        }
    }

    void GUI::CleanupRenderTarget() {
        if (render_target) {
            render_target->Release();
            render_target = nullptr;
        }
    }

    LRESULT WINAPI GUI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        if (msg == WM_NCCREATE) {
            auto* cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
            SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
        }
        auto* self = reinterpret_cast<GUI*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));

        switch (msg) {
        case WM_SIZE:
            if (self->device && wParam != SIZE_MINIMIZED)
                self->ResizeWindow((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU)
                return 0;
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }

    void GUI::ResizeWindow(int width, int height) {
        CleanupRenderTarget();
        swapchain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
        CreateRenderTarget();
    }

    void GUI::Run() {
        ImGui_ImplWin32_EnableDpiAwareness();

        WNDCLASSEXW window_class = { sizeof(WNDCLASSEXW), CS_CLASSDC, WndProc, 0, 0, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"FTAP-Ware", nullptr };
        RegisterClassExW(&window_class);

        HWND window = CreateWindowExW(
            WS_EX_APPWINDOW,
            window_class.lpszClassName, window_class.lpszClassName, WS_POPUP, 0, 0, 0, 0, nullptr, nullptr, window_class.hInstance, this);

        if (!CreateDevice(window) || !CreateRenderTarget()) {
            CleanupRenderTarget();
            CleanupDevice();

            DestroyWindow(window);
            UnregisterClassW(window_class.lpszClassName, window_class.hInstance);
            return;
        }

        ShowWindow(window, SW_HIDE);
        UpdateWindow(window);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();
        ImGuiStyle& st = ImGui::GetStyle();
        st.WindowRounding = 0.0f;
        st.WindowBorderSize = 0.0f;

        ImGui_ImplWin32_Init(window);
        ImGui_ImplDX11_Init(device, device_context);

        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

        MSG msg{};
        running = true;
        while (running) {
            while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    running = false;
                    break;
                }

                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }

            if (keybind_listener_enabled)
                keybind_manager.Update();

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            Draw();

            ImGui::Render();
            const float clear_color_with_alpha[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            device_context->OMSetRenderTargets(1, &render_target, nullptr);
            device_context->ClearRenderTargetView(render_target, clear_color_with_alpha);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            swapchain->Present(vsync, 0);
        }

        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        if (ImGui::GetCurrentContext()) ImGui::DestroyContext();

        CleanupRenderTarget();
        CleanupDevice();

        DestroyWindow(window);
        UnregisterClassW(window_class.lpszClassName, window_class.hInstance);
    }

    void GUI::Stop() {
        running = false;
    }

    void GUI::Draw() {
        ImGui::Begin("FTAP-Ware", &running);

        if (ImGui::BeginTabBar("TabBar", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_TabListPopupButton)) {
            if (ImGui::BeginTabItem("Main")) {
                {
                    ImGui::SeparatorText("AutoGrab");
                    ImGui::PushID("AutoGrab");

                    {
                        bool enabled = engine.IsAutoGrabEnabled();
                        if (ImGui::Checkbox("Enable", &enabled)) {
                            engine.SetAutoGrabEnabled(enabled);
                        }
                    }

                    // ImGui::SameLine();

                    // {
                    //     ImGui::Checkbox("FOV Check", &client.settings.autograb.fov_check);
                    // }

                    // ImGui::SameLine();

                    // {
                    //     ImGui::Checkbox("Wall Check", &client.settings.autograb.wall_check);
                    // }

                    {
                        int key = keybind_manager.GetKey("autograb.toggle");
                        if (Draw::KeyBindButton("Toggle Key", key)) {
                            keybind_manager.SetKey("autograb.toggle", key);
                        }
                    }

                    ImGui::SameLine();

                    {
                        int key = keybind_manager.GetKey("autograb.lock");

                        if (Draw::KeyBindButton("Lock Key", key)) {
                            keybind_manager.SetKey("autograb.lock", key);
                        };
                    }

                    {
                        std::string locked_target = engine.GetAutoGrabTarget();
                        ImGui::Text("Locked target: %s", locked_target.c_str());
                    }

                    ImGui::PopID();
                }

                {
                    ImGui::SeparatorText("TabGlitch");

                    {

                        int mode = static_cast<int>(engine.GetTabGlitchMode());

                        if (ImGui::Combo("Mode", &mode, tabglitch_modes, IM_ARRAYSIZE(tabglitch_modes))) {
                            engine.SetTabGlitchMode(
                                static_cast<Modules::TabGlitch::Mode>(mode)
                            );
                        }
                    }

                    {
                        int key = keybind_manager.GetKey("tabglitch.trigger");
                        if (Draw::KeyBindButton("Trigger Key", key)) {
                            keybind_manager.SetKey("tabglitch.trigger", key);
                        }
                    }
                }

                {
                    ImGui::SeparatorText("FOV");

                    int fov = engine.GetFOV();

                    if (ImGui::SliderInt("##fov", &fov, 1, 180)) {
                        engine.SetFOV(fov);
                    }
                }

                {
                    ImGui::SeparatorText("PalletControl");
                    ImGui::PushID("PalletControl");

                    {
                        bool enabled = engine.IsPalletControlEnabled();
                        if (ImGui::Checkbox("Enable", &enabled)) {
                            engine.SetPalletControlEnabled(enabled);
                        }
                    }

                    {
                        int key = keybind_manager.GetKey("palletcontrol.toggle");
                        if (Draw::KeyBindButton("Toggle Key", key)) {
                            keybind_manager.SetKey("palletcontrol.toggle", key);
                        }
                    }

                    ImGui::PopID();
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Processes")) {
                ImGui::TextUnformatted("Process Manager");
                ImGui::SameLine();

                ImGui::TextDisabled("(%zu processes)", process_ids.size());

                ImGui::SameLine(ImGui::GetContentRegionAvail().x - 75.0f);

                if (ImGui::Button("Refresh", ImVec2(75.0f, 0.0f))) {
                    process_ids = Win32::FindProcessIds(L"RobloxPlayerBeta.exe");
                    selected_process = -1;
                }

                ImGui::Separator();
                ImGui::Spacing();

                ImGui::TextDisabled("AVAILABLE PROCESSES");
                ImGui::Spacing();

                if (process_ids.empty()) {
                    ImGui::TextDisabled("No Roblox processes found.");
                }
                else {
                    ImGui::BeginChild(
                        "ProcessList",
                        ImVec2(0.0f, 220.0f),
                        ImGuiChildFlags_Borders
                    );

                    for (int i = 0; i < static_cast<int>(process_ids.size()); ++i) {
                        const uint32_t pid = process_ids[i];

                        ImGui::PushID(static_cast<int>(pid));

                        const bool selected = selected_process == i;

                        ImGui::PushStyleVar(
                            ImGuiStyleVar_FramePadding,
                            ImVec2(10.0f, 10.0f)
                        );

                        if (ImGui::Selectable(
                            "##process",
                            selected,
                            ImGuiSelectableFlags_None,
                            ImVec2(0.0f, 42.0f))) {
                            selected_process = i;
                        }

                        ImGui::PopStyleVar();

                        ImGui::SameLine(12.0f);

                        ImGui::BeginGroup();
                        ImGui::Text("Roblox Player");
                        ImGui::TextDisabled("PID: %u", pid);
                        ImGui::EndGroup();

                        ImGui::PopID();
                    }

                    ImGui::EndChild();
                }

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::TextDisabled("PROCESS ACTIONS");
                ImGui::Spacing();

                const float button_width = (ImGui::GetContentRegionAvail().x - 8.0f) / 2.0f;

                if (selected_process >= 0 &&
                    selected_process < static_cast<int>(process_ids.size())) {

                    const uint32_t pid = process_ids[selected_process];

                    if (ImGui::Button("Attach", ImVec2(button_width, 36.0f))) {
                        auto result = engine.Attach(pid);
                    }
                }
                else {
                    ImGui::BeginDisabled();
                    ImGui::Button("Attach", ImVec2(button_width, 36.0f));
                    ImGui::EndDisabled();
                }

                ImGui::SameLine();

                if (engine.IsAttached()) {
                    if (ImGui::Button("Detach", ImVec2(button_width, 36.0f))) {
                        engine.Detach();
                    }
                }
                else {
                    ImGui::BeginDisabled();
                    ImGui::Button("Detach", ImVec2(button_width, 36.0f));
                    ImGui::EndDisabled();
                }

                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("Logs")) {
                ImGui::TextUnformatted("Debug Logs");

                ImGui::SameLine();

                {
                    std::lock_guard lock(log_mutex);
                    ImGui::TextDisabled("(%zu entries)", logs.size());
                }


                const float button_width =
                    ImGui::CalcTextSize("Clear").x +
                    ImGui::GetStyle().FramePadding.x * 2.0f;

                const float checkbox_width =
                    ImGui::GetFrameHeight() +
                    ImGui::GetStyle().ItemInnerSpacing.x +
                    ImGui::CalcTextSize("Auto-scroll").x;

                const float total_width =
                    button_width +
                    ImGui::GetStyle().ItemSpacing.x +
                    checkbox_width;

                ImGui::SameLine(
                    ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - total_width
                );

                if (ImGui::Button("Clear")) {
                    std::lock_guard lock(log_mutex);
                    logs.clear();
                }

                ImGui::SameLine();

                ImGui::Checkbox("Auto-scroll", &auto_scroll);

                ImGui::Separator();

                ImGui::BeginChild(
                    "LogEntries",
                    ImVec2(0.0f, 0.0f),
                    ImGuiChildFlags_Borders,
                    ImGuiWindowFlags_HorizontalScrollbar
                );

                std::vector<LogEntry> snapshot;

                {
                    std::lock_guard lock(log_mutex);
                    snapshot = logs;
                }

                const bool was_at_bottom =
                    ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 1.0f;

                for (const auto& entry : snapshot) {
                    switch (entry.level) {
                    case LogLevel::Info:
                        ImGui::TextDisabled("[INFO]");
                        break;

                    case LogLevel::Warning:
                        ImGui::TextColored(
                            ImVec4(1.0f, 0.75f, 0.25f, 1.0f),
                            "[WARNING]"
                        );
                        break;

                    case LogLevel::Error:
                        ImGui::TextColored(
                            ImVec4(1.0f, 0.35f, 0.35f, 1.0f),
                            "[ERROR]"
                        );
                        break;
                    }

                    ImGui::SameLine();
                    ImGui::TextUnformatted(entry.message.c_str());
                }

                if (auto_scroll && was_at_bottom)
                    ImGui::SetScrollHereY(1.0f);

                ImGui::EndChild();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Settings")) {
                ImGui::Checkbox("Keybind Listener", &keybind_listener_enabled);
                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();
        ImGui::End();
    }
}