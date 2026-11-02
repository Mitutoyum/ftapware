#include <cstdint>

namespace Offsets {
    namespace FakeDataModel {
        inline constexpr uintptr_t Pointer = 0x8d06600;
        inline constexpr uintptr_t RealDataModel = 0x1f8;
    }

    namespace DataModel {
        inline constexpr uintptr_t GameLoaded = 0x588;
    }

    namespace Instance {
        inline constexpr uintptr_t ChildrenEnd = 0x8;
        inline constexpr uintptr_t ChildrenStart = 0x70;
        inline constexpr uintptr_t ClassDescriptor = 0x18;
        inline constexpr uintptr_t ClassName = 0x8;
        inline constexpr uintptr_t ClassBase = 0x1b0;
        inline constexpr uintptr_t Name = 0x0;
        inline constexpr uintptr_t NameContainer = 0x68;
        inline constexpr uintptr_t Parent = 0x60;
    }

    namespace BasePart {
        inline constexpr uintptr_t Color3 = 0x11c;
        inline constexpr uintptr_t Shape = 0x1a0;
        inline constexpr uintptr_t Primitive = 0x178;
    }

    namespace Primitive {
        inline constexpr uintptr_t AssemblyAngularVelocity = 0xec;
        inline constexpr uintptr_t AssemblyLinearVelocity = 0xe0;
        inline constexpr uintptr_t Material = 0x0;
        inline constexpr uintptr_t Position = 0xd4;
        inline constexpr uintptr_t Rotation = 0xb0;
        inline constexpr uintptr_t Size = 0x1c4;
    }

    namespace Camera {
        inline constexpr uintptr_t CameraSubject = 0xb0;
        inline constexpr uintptr_t CameraType = 0x120;
        inline constexpr uintptr_t FieldOfView = 0x128;
        inline constexpr uintptr_t Position = 0xe4;
        inline constexpr uintptr_t Rotation = 0xc0;
    }

    namespace Humanoid {
        inline constexpr uintptr_t FloorMaterial = 0x178;
    }

    namespace ValueBase {
        inline constexpr uintptr_t Value = 0xa0;
    }

    namespace MouseService {
        inline constexpr uintptr_t InputObject = 0xe8;
    }

    namespace InputObject {
        inline constexpr uintptr_t MousePosition = 0xbc;
    }

    namespace Players {
        inline constexpr uintptr_t LocalPlayer = 0x118;
    }

    namespace Player {
        inline constexpr uintptr_t ModelInstance = 0x2a0;
    }

    namespace VisualEngine {
        inline constexpr uintptr_t Dimensions = 0xb10;
        inline constexpr uintptr_t Pointer = 0x879a218;
        inline constexpr uintptr_t ViewMatrix = 0x1b0;
    }

    namespace WeldConstraint {
        inline constexpr uintptr_t Part0 = 0xa0;
        inline constexpr uintptr_t Part1 = 0xb0;
    }

    namespace Attachment {
        inline constexpr uintptr_t Position = 0xac;
    }
}