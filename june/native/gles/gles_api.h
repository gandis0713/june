#pragma once

#include <EGL/egl.h>
#include <EGL/eglext.h>

namespace june
{

#define CHECK_GL_ERROR()                                                   \
    {                                                                      \
        GLenum err = glGetError();                                         \
        if (err != GL_NO_ERROR)                                            \
        {                                                                  \
            spdlog::error("GL get error: {}", static_cast<uint32_t>(err)); \
        }                                                                  \
    }

class DyLib;
struct GLESAPI
{
    bool loadClientProcs(DyLib* glesLib);
    bool loadDisplayProcs(EGLDisplay display);

    uint32_t getMajorVersion() const
    {
        return m_majorVersion;
    }
    uint32_t getMinorVersion() const
    {
        return m_minorVersion;
    }

    // EGL 1.0
    PFNEGLGETPROCADDRESSPROC GetProcAddress;

    PFNEGLCHOOSECONFIGPROC ChooseConfig;
    PFNEGLCOPYBUFFERSPROC CopyBuffers;
    PFNEGLCREATECONTEXTPROC CreateContext;
    PFNEGLCREATEPBUFFERSURFACEPROC CreatePbufferSurface;
    PFNEGLCREATEPIXMAPSURFACEPROC CreatePixmapSurface;
    PFNEGLCREATEWINDOWSURFACEPROC CreateWindowSurface;
    PFNEGLDESTROYCONTEXTPROC DestroyContext;
    PFNEGLDESTROYSURFACEPROC DestroySurface;
    PFNEGLGETCONFIGATTRIBPROC GetConfigAttrib;
    PFNEGLGETCONFIGSPROC GetConfigs;
    PFNEGLGETCURRENTDISPLAYPROC GetCurrentDisplay;
    PFNEGLGETCURRENTSURFACEPROC GetCurrentSurface;
    PFNEGLGETDISPLAYPROC GetDisplay;
    PFNEGLGETERRORPROC GetError;
    PFNEGLINITIALIZEPROC Initialize;
    PFNEGLMAKECURRENTPROC MakeCurrent;
    PFNEGLQUERYCONTEXTPROC QueryContext;
    PFNEGLQUERYSTRINGPROC QueryString;
    PFNEGLQUERYSURFACEPROC QuerySurface;
    PFNEGLSWAPBUFFERSPROC SwapBuffers;
    PFNEGLTERMINATEPROC Terminate;
    PFNEGLWAITGLPROC WaitGL;
    PFNEGLWAITNATIVEPROC WaitNative;

    // EGL 1.1
    PFNEGLBINDTEXIMAGEPROC BindTexImage;
    PFNEGLRELEASETEXIMAGEPROC ReleaseTexImage;
    PFNEGLSURFACEATTRIBPROC SurfaceAttrib;
    PFNEGLSWAPINTERVALPROC SwapInterval;

    // EGL 1.2
    PFNEGLBINDAPIPROC BindAPI;
    PFNEGLQUERYAPIPROC QueryAPI;
    PFNEGLCREATEPBUFFERFROMCLIENTBUFFERPROC CreatePbufferFromClientBuffer;
    PFNEGLRELEASETHREADPROC ReleaseThread;
    PFNEGLWAITCLIENTPROC WaitClient;

    // EGL 1.3 (no new procs)

    // EGL 1.4
    PFNEGLGETCURRENTCONTEXTPROC GetCurrentContext;

    // EGL 1.5
    PFNEGLCREATESYNCPROC CreateSync;
    PFNEGLDESTROYSYNCPROC DestroySync;
    PFNEGLCLIENTWAITSYNCPROC ClientWaitSync;
    PFNEGLGETSYNCATTRIBPROC GetSyncAttrib;
    PFNEGLCREATEIMAGEPROC CreateImage;
    PFNEGLDESTROYIMAGEPROC DestroyImage;
    PFNEGLGETPLATFORMDISPLAYPROC GetPlatformDisplay;
    PFNEGLCREATEPLATFORMWINDOWSURFACEPROC CreatePlatformWindowSurface;
    PFNEGLCREATEPLATFORMPIXMAPSURFACEPROC CreatePlatformPixmapSurface;
    PFNEGLWAITSYNCPROC WaitSync;

    // EGL_KHR_fence_sync
    // NOTE: These functions use attribute lists with EGLint but the core versions use EGLattrib.
    // They are not compatible.
    PFNEGLCREATESYNCKHRPROC CreateSyncKHR;
    PFNEGLDESTROYSYNCKHRPROC DestroySyncKHR;
    PFNEGLCLIENTWAITSYNCKHRPROC ClientWaitSyncKHR;
    PFNEGLGETSYNCATTRIBKHRPROC GetSyncAttribKHR;

    // EGL_KHR_reusable_sync
    PFNEGLSIGNALSYNCKHRPROC SignalSync;

    // EGL_ANDROID_get_native_client_buffer
    PFNEGLGETNATIVECLIENTBUFFERANDROIDPROC GetNativeClientBuffer;

    // EGL_ANDROID_native_fence_sync
    PFNEGLDUPNATIVEFENCEFDANDROIDPROC DupNativeFenceFD;

private:
    uint32_t m_majorVersion{ 0 };
    uint32_t m_minorVersion{ 0 };
};

} // namespace june