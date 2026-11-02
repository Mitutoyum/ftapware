#pragma once

#include "caches.h"

#include "ftapware/engine/client/memory/roblox/camera.h"
#include "ftapware/engine/client/memory/roblox/player.h"
#include "ftapware/engine/client/memory/roblox/players.h"
#include "ftapware/engine/client/memory/roblox/instance.h"
#include "ftapware/engine/client/memory/roblox/datamodel.h"
#include "ftapware/engine/client/memory/roblox/inputobject.h"
#include "ftapware/engine/client/memory/roblox/visualengine.h"
#include "ftapware/engine/client/memory/roblox/mouseservice.h"

#include <expected>
#include <memory>
#include <string>

namespace FTAPWare {
    class GameSession {
    private:
        const Process& process;
        Caches caches;
        
        Roblox::Camera camera;
        Roblox::Players players;
        Roblox::Instance workspace;
        Roblox::Player localplayer;
        Roblox::DataModel datamodel;
        Roblox::InputObject inputobject;
        Roblox::VisualEngine visualengine;
        Roblox::MouseService mouseservice;
    
    private:
        GameSession(
            const Process& process,
            Roblox::Camera camera,
            Roblox::Players players,
            Roblox::Instance workspace,
            Roblox::Player localplayer,
            Roblox::DataModel datamodel,
            Roblox::InputObject inputobject,
            Roblox::VisualEngine visualengine,
            Roblox::MouseService mouseservice
        );
    public: 
        static std::expected<std::unique_ptr<GameSession>, std::string> Create(const Process& process);
        
        const Roblox::Camera& GetCamera() const;
        const Roblox::Instance& GetPlayers() const;
        const Roblox::Instance& GetWorkspace() const;
        const Roblox::Player& GetLocalPlayer() const;
        const Roblox::DataModel& GetDataModel() const;
        const Roblox::InputObject& GetInputObject() const;
        const Roblox::VisualEngine& GetVisualEngine() const;
        const Roblox::MouseService& GetMouseService() const;

        Caches& GetCaches();
    };
}