#pragma once

#include <memory>

#include "process.h"
#include "teleport_handler.h"
#include "game_session/game_session.h"

namespace FTAPWare {
    class Client {
    private:
        Process process;
        TeleportHandler teleport_handler;
        std::unique_ptr<GameSession> game_session { nullptr };
    public:
        Client(uint32_t PID);

        const Process& GetProcess() const;
        GameSession* GetGameSession() const;
        const TeleportHandler& GetTeleportHandler() const;
    };
}