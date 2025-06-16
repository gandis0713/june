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

## Features

The header [`june.h`](june/june.h) exposes a minimal C interface for working
with external GPU resources. Highlights include:

- Unified handling of external memory and synchronization primitives.
- Support for Vulkan, D3D11, D3D12, OpenGL, GLES and Metal backends.
- Opaque handles for instances, API contexts, shared memory and fences.
- Descriptor structures use a `JuneChainedStruct` header so additional
  platform specific information can be chained when required.
- Resources such as Vulkan buffers or EGL images can be created from imported
  memory.
- Shared memory is always imported from platform handles and cannot be
  created directly by the library.
- Fence objects are created and reset using imported synchronization handles
  (e.g. EGLSync or SyncFD) and can be exported to synchronize work between
  different APIs.
- Platform-specific extensions cover handles like AHardwareBuffer, DMABuf,
  IOSurface and fences such as MTLSharedFence.

