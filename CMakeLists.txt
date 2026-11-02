cmake_minimum_required(VERSION 3.20)

project(FTAPWare
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(imgui CONFIG REQUIRED)

add_executable(FTAPWare WIN32
    src/main.cpp

    src/ftapware/application.cpp

    src/ftapware/engine/engine.cpp
    src/ftapware/engine/context.cpp
    src/ftapware/engine/module_manager.cpp

    src/ftapware/engine/modules/fov.cpp
    src/ftapware/engine/modules/module.cpp
    src/ftapware/engine/modules/autograb.cpp
    src/ftapware/engine/modules/tabglitch.cpp
    src/ftapware/engine/modules/palletcontrol.cpp


    src/ftapware/engine/client/client.cpp
    src/ftapware/engine/client/process.cpp
    src/ftapware/engine/client/teleport_handler.cpp

    src/ftapware/engine/client/common/player.cpp

    src/ftapware/engine/client/game_session/game_session.cpp
    src/ftapware/engine/client/game_session/caches.cpp

    
    src/ftapware/engine/client/memory/remote_object.cpp

    src/ftapware/engine/client/memory/roblox/instance.cpp
    src/ftapware/engine/client/memory/roblox/datamodel.cpp
    src/ftapware/engine/client/memory/roblox/player.cpp
    src/ftapware/engine/client/memory/roblox/modelinstance.cpp
    src/ftapware/engine/client/memory/roblox/attachment.cpp
    src/ftapware/engine/client/memory/roblox/basepart.cpp
    src/ftapware/engine/client/memory/roblox/visualengine.cpp
    src/ftapware/engine/client/memory/roblox/weldconstraint.cpp
    src/ftapware/engine/client/memory/roblox/inputobject.cpp
    src/ftapware/engine/client/memory/roblox/humanoid.cpp
    src/ftapware/engine/client/memory/roblox/camera.cpp
    src/ftapware/engine/client/memory/roblox/mouseservice.cpp
    src/ftapware/engine/client/memory/roblox/players.cpp
    src/ftapware/engine/client/memory/roblox/primitive.cpp
    src/ftapware/engine/client/memory/roblox/boolvalue.cpp

    src/ftapware/common/win32.cpp
    src/ftapware/common/math/utils.cpp
    src/ftapware/common/math/cframe.cpp
    src/ftapware/common/math/vector2.cpp
    src/ftapware/common/math/vector3.cpp
    src/ftapware/common/math/matrix3x3.cpp
    src/ftapware/common/math/matrix4x4.cpp
    src/ftapware/common/math/quaternion.cpp
    src/ftapware/common/cache/lazycache.cpp
    src/ftapware/common/cache/periodiccache.cpp

    src/ftapware/gui/gui.cpp
    src/ftapware/gui/common/draw.cpp
    src/ftapware/gui/keybind_manager.cpp
    src/ftapware/gui/logstreambuf.cpp

    src/ftapware/application.cpp
)

target_compile_features(FTAPWare PRIVATE cxx_std_23)

set_target_properties(FTAPWare PROPERTIES
    CXX_EXTENSIONS OFF
)

target_include_directories(FTAPWare PRIVATE src/)

target_compile_definitions(FTAPWare PRIVATE
    NOMINMAX
)

target_link_libraries(FTAPWare PRIVATE
    imgui::imgui
    ntdll
    d3d11
    dxgi
)