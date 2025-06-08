# June
External Memory & Synchronization Abstraction Library

## Building

This project uses [vcpkg](https://github.com/microsoft/vcpkg) for all
dependencies. Ensure `VCPKG_ROOT` points to your vcpkg installation and that
the required packages from `vcpkg.json` are installed. Configure and build
using one of the presets defined in `CMakePresets.json`:

```sh
cmake --preset x64-osx-ninja-debug
cmake --build --preset x64-osx-ninja-debug
```

