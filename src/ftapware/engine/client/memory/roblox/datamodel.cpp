#include "datamodel.h"

#include "ftapware/engine/client/memory/offsets.h"

namespace FTAPWare::Roblox {
    DataModel::DataModel(const Instance& instance) : Instance(instance) {}

    DataModel DataModel::Get(const Process& process) {
        uintptr_t fake_datamodel = process.Read<uintptr_t>(process.GetBaseAddress() + Offsets::FakeDataModel::Pointer);

        if (!fake_datamodel) return DataModel();

        return DataModel(process.Read<uintptr_t>(fake_datamodel + Offsets::FakeDataModel::RealDataModel), &process);
    }

    bool DataModel::IsGameLoaded() const {
        if (!IsValid()) return false;

        return ReadFrom<uint64_t>(Offsets::DataModel::GameLoaded) == 31;
    }
}