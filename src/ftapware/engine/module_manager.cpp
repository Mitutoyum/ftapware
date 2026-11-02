#include "module_manager.h"
#include "modules/module.h"


namespace FTAPWare {
    void ModuleManager::AddModule(std::unique_ptr<Modules::Module> module) {
        modules.push_back(std::move(module));
    }
}