#pragma once

#include <string>
#include <memory>
#include <vector>

namespace FTAPWare::Modules {
    class Module;
}

namespace FTAPWare {
    class ModuleManager {
    private:
        std::vector<std::unique_ptr<Modules::Module>> modules;
    public:
        void AddModule(std::unique_ptr<Modules::Module> module);

        template <typename T>
        T* GetModule() const {
            for (const auto& module : modules) {
                if (auto* result = dynamic_cast<T*>(module.get()))
                    return result;
            }

            return nullptr;
        }
    };
}