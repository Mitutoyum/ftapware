#pragma once

#include <atomic>

namespace FTAPWare {
    class Client;

    class Context {
    private:
        std::atomic<Client*> client{ nullptr };
    public:
        void SetClient(Client* client);
        Client* GetClient() const;
    };
}