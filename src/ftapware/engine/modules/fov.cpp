#include "fov.h"

#include "ftapware/engine/context.h"
#include "ftapware/engine/client/client.h"

namespace FTAPWare::Modules {
    std::string FOV::GetName() const {
        return "FOV";
    }

    void FOV::SetFOV(float FOV) {
        fov = FOV;

        auto client = context.GetClient();

        if (!client) {
            return;
        }

        auto game_session = client->GetGameSession();

        if (!game_session) {
            return;
        }

        auto camera = game_session->GetCamera();
        camera.SetFieldOfView(FOV);
    }

    float FOV::GetFOV() const {
        return fov;
    }
}
