#include "game_session.h"
#include "process.h"

namespace FTAPWare {
    std::expected<std::unique_ptr<GameSession>, std::string> GameSession::Create(const Process& process) {
        Roblox::DataModel datamodel = Roblox::DataModel::Get(process);

        if (!datamodel.IsValid()) {
            return std::unexpected("DataModel not found");
        }

        Roblox::VisualEngine visualengine = Roblox::VisualEngine::Get(process);

        if (!visualengine.IsValid()) {
            return std::unexpected("VisualEngine not found");
        }

        Roblox::Instance workspace = datamodel.FindFirstChild("Workspace");
        
        if (!workspace.IsValid()) {
            return std::unexpected("Workspace not found");
        }

        Roblox::Players players = datamodel.FindFirstChild("Players");

        if (!players.IsValid()) {
            return std::unexpected("Players not found");
        }

        Roblox::MouseService mouseservice = datamodel.FindFirstChild("MouseService");

        if (!mouseservice.IsValid()) {
            return std::unexpected("MouseService not found");
        }

        Roblox::Camera camera = workspace.FindFirstChild("Camera");

        if (!camera.IsValid()) {
            return std::unexpected("Camera not found");
        }

        Roblox::Player localplayer = players.GetLocalPlayer();

        if (!localplayer.IsValid()) {
            return std::unexpected("LocalPlayer not found");
        }

        Roblox::InputObject inputobject = mouseservice.GetInputObject();

        if (!inputobject.IsValid()) {
            return std::unexpected("InputObject not found");
        }

        return std::unique_ptr<GameSession>(new GameSession(
            process,
            std::move(camera),
            std::move(players),
            std::move(workspace),
            std::move(localplayer),
            std::move(datamodel),
            std::move(inputobject),
            std::move(visualengine),
            std::move(mouseservice)
        ));
    }

    GameSession::GameSession(
        const Process& process,
        Roblox::Camera camera,
        Roblox::Players players,
        Roblox::Instance workspace,
        Roblox::Player localplayer,
        Roblox::DataModel datamodel,
        Roblox::InputObject inputobject,
        Roblox::VisualEngine visualengine,
        Roblox::MouseService mouseservice
    ) : process(process),
        camera(std::move(camera)),
        players(std::move(players)),
        workspace(std::move(workspace)),
        localplayer(std::move(localplayer)),
        datamodel(std::move(datamodel)),
        inputobject(std::move(inputobject)),
        visualengine(std::move(visualengine)),
        mouseservice(std::move(mouseservice)),
        caches(this->visualengine, this->players, this->localplayer) {}

    const Roblox::Camera& GameSession::GetCamera() const {
        return camera;
    }

    const Roblox::Instance& GameSession::GetPlayers() const {
        return players;
    }
    const Roblox::Instance& GameSession::GetWorkspace() const {
        return workspace;
    }

    const Roblox::Player& GameSession::GetLocalPlayer() const {
        return localplayer;
    }

    const Roblox::DataModel& GameSession::GetDataModel() const {
        return datamodel;
    }

    const Roblox::InputObject& GameSession::GetInputObject() const {
        return inputobject;
    }

    const Roblox::VisualEngine& GameSession::GetVisualEngine() const {
        return visualengine;
    }

    const Roblox::MouseService& GameSession::GetMouseService() const {
        return mouseservice;
    }

    Caches& GameSession::GetCaches() {
        return caches;
    }
}