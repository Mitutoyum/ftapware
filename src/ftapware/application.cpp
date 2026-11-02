#include "application.h"
#include <iostream>

namespace FTAPWare {
    Application::Application() : gui(engine) {}

    void Application::Run() {
        gui.Run();
    }

    void Application::Stop() {
        gui.Stop();
    }
}