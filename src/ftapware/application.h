#pragma once

#include "engine/engine.h"
#include "gui/gui.h"

namespace FTAPWare {
    class Application {
    private:
        Engine engine;
        GUI gui;
    public:
        Application();
        
        void Run();
        void Stop();
    };
}