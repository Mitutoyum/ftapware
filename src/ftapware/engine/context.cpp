#include "context.h"

#include "client/client.h"

namespace FTAPWare {
    Client* Context::GetClient() const {
        return client.load(std::memory_order_acquire);
    }
    void Context::SetClient(Client* client) {
        this->client.store(client, std::memory_order_release);
    }
}