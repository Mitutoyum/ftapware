#include "client.h"
#include "process.h"
#include "teleport_handler.h"

#include <iostream>

namespace FTAPWare {
    Client::Client(uint32_t PID) : process(PID), teleport_handler(process) {
        teleport_handler.AddEvent("game_session_join", TeleportHandler::EventType::Join, [this]() {
            auto game_session = GameSession::Create(process);

            if (!game_session) {
                return;
            }

            this->game_session = std::move(*game_session);
        });

        teleport_handler.AddEvent("game_session_leave", TeleportHandler::EventType::Leave, [this]() {
            game_session.reset();
        });

        teleport_handler.Start();
    }

    const Process& Client::GetProcess() const {
        return process;
    }

    GameSession* Client::GetGameSession() const {
        return game_session.get();
    }

    const TeleportHandler& Client::GetTeleportHandler() const {
        return teleport_handler;
    }
}