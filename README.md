# FTAPWare

FTAPWare is an external utility for the game Fling Things and People on the Roblox platform designed to give competitive players an easier gameplay experience

## Features

* Auto Grab
* Pallet Control
* FOV Changer
* Tab Glitch

## Deployment

### Requirements

* Windows 10/11 (64-bit)
* Visual Studio 2026 with C++ desktop development tools
* [CMake](https://cmake.org/)
* [vcpkg](https://github.com/microsoft/vcpkg)
* [git](https://git-scm.com/)

### Build Instructions

**1. Clone the repository**

```bash
git clone https://github.com/Mitutoyum/FTAPWare.git
cd FTAPWare
```

**2. Configure the project**

```bash
cmake --preset default
```

**3. Build**

```bash
cmake --build build/
```