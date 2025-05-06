#include "gles_api.h"

#include "june/common/dylib.h"
#include <spdlog/spdlog.h>

namespace june
{

// #define GET_PROC_WITH_NAME(member, name)                                          \
//     do                                                                            \
//     {                                                                             \
//         member = reinterpret_cast<decltype(member)>(GetProcAddress(name));        \
//         if (member == nullptr)                                                    \
//         {                                                                         \
//             return DAWN_INTERNAL_ERROR(std::string("Couldn't get proc ") + name); \
//         }                                                                         \
//     } while (0)

// #define GET_PROC(name) GET_PROC_WITH_NAME(name, "egl" #name)

#define GET_PROC(name)                                                    \
    name = reinterpret_cast<decltype(name)>(GetProcAddress("egl" #name)); \
    if (name == nullptr)                                                  \
    {                                                                     \
        spdlog::error("Couldn't get proc egl{}", #name);                  \
        return false;                                                     \
    }

bool GLESAPI::loadClientProcs(DyLib* glesLib)
{
    if (glesLib == nullptr)
    {
        GetProcAddress = eglGetProcAddress;
    }
    else
    {
        if (!glesLib->getProc(&GetProcAddress, "eglGetProcAddress"))
        {
            spdlog::error("Couldn't get eglGetProcAddress");
            return false;
        }
    }

    GET_PROC(ChooseConfig);
    GET_PROC(CopyBuffers);
    GET_PROC(CreateContext);
    GET_PROC(CreatePbufferSurface);
    GET_PROC(CreatePixmapSurface);
    GET_PROC(CreateWindowSurface);
    GET_PROC(DestroyContext);
    GET_PROC(DestroySurface);
    GET_PROC(GetConfigAttrib);
    GET_PROC(GetConfigs);
    GET_PROC(GetCurrentDisplay);
    GET_PROC(GetCurrentSurface);
    GET_PROC(GetDisplay);
    GET_PROC(GetError);
    GET_PROC(Initialize);
    GET_PROC(MakeCurrent);
    GET_PROC(QueryContext);
    GET_PROC(QueryString);
    GET_PROC(QuerySurface);
    GET_PROC(SwapBuffers);
    GET_PROC(Terminate);
    GET_PROC(WaitGL);
    GET_PROC(WaitNative);

    return true;
}

bool GLESAPI::loadDisplayProcs(EGLDisplay display)
{
    if (display == nullptr || display == EGL_NO_DISPLAY)
    {
        spdlog::warn("EGL display is invalid");
        return false;
    }

    EGLint major = 0;
    EGLint minor = 0;
    Initialize(display, &major, &minor);

    spdlog::debug("EGL version: {}.{}", major, minor);

    m_majorVersion = major;
    m_minorVersion = minor;

    // EGL 1.1
    if (m_minorVersion >= 1)
    {
        GET_PROC(BindTexImage);
        GET_PROC(ReleaseTexImage);
        GET_PROC(SurfaceAttrib);
        GET_PROC(SwapInterval);
    }

    // EGL 1.2
    if (m_minorVersion >= 2)
    {
        GET_PROC(BindAPI);
        GET_PROC(QueryAPI);
        GET_PROC(CreatePbufferFromClientBuffer);
        GET_PROC(ReleaseThread);
        GET_PROC(WaitClient);
    }

    // EGL 1.3 (no new procs)

    // EGL 1.4
    if (m_minorVersion >= 4)
    {
        GET_PROC(GetCurrentContext);
    }

    // EGL 1.5
    if (m_minorVersion >= 5)
    {
        GET_PROC(CreateSync);
        GET_PROC(DestroySync);
        GET_PROC(ClientWaitSync);
        GET_PROC(GetSyncAttrib);
        GET_PROC(CreateImage);
        GET_PROC(DestroyImage);
        GET_PROC(GetPlatformDisplay);
        GET_PROC(CreatePlatformWindowSurface);
        GET_PROC(CreatePlatformPixmapSurface);
        GET_PROC(WaitSyncKHR);
    }

    {
        // Load display extensions that would otherwise be promoted to EGL 1.5.

        // if (HasExt(EGLExt::ImageBase))
        // {
        GET_PROC(CreateImageKHR);
        GET_PROC(DestroyImageKHR);
        // }

        // if (HasExt(EGLExt::WaitSync))
        // {
        GET_PROC(WaitSyncKHR)
        // }
    }

    // // Load client extensions if they haven't been already.
    // if (!HasExt(EGLExt::ClientExtensions))
    // {
    //     DAWN_TRY(LoadClientExtensions());
    // }

    // // Other extensions

    // if (HasExt(EGLExt::FenceSync))
    // {
    GET_PROC(ClientWaitSyncKHR)
    GET_PROC(CreateSyncKHR);
    GET_PROC(DestroySyncKHR);
    GET_PROC(GetSyncAttribKHR);
    // }

    // if (HasExt(EGLExt::ReusableSync))
    // {
    GET_PROC(SignalSyncKHR);
    // }

    // if (HasExt(EGLExt::GetNativeClientBuffer))
    // {
    GET_PROC(GetNativeClientBufferANDROID);
    // }

    // if (HasExt(EGLExt::NativeFenceSync))
    // {
    GET_PROC(DupNativeFenceFDANDROID);
    // }

    return true;
}

} // namespace june