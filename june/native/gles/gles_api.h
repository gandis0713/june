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

// EGL 1.0
typedef EGLBoolean(EGLAPIENTRYP PFNEGLCHOOSECONFIGPROC)(EGLDisplay dpy, const EGLint* attrib_list, EGLConfig* configs, EGLint config_size, EGLint* num_config);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLCOPYBUFFERSPROC)(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target);
typedef EGLContext(EGLAPIENTRYP PFNEGLCREATECONTEXTPROC)(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint* attrib_list);
typedef EGLSurface(EGLAPIENTRYP PFNEGLCREATEPBUFFERSURFACEPROC)(EGLDisplay dpy, EGLConfig config, const EGLint* attrib_list);
typedef EGLSurface(EGLAPIENTRYP PFNEGLCREATEPIXMAPSURFACEPROC)(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint* attrib_list);
typedef EGLSurface(EGLAPIENTRYP PFNEGLCREATEWINDOWSURFACEPROC)(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint* attrib_list);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLDESTROYCONTEXTPROC)(EGLDisplay dpy, EGLContext ctx);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLDESTROYSURFACEPROC)(EGLDisplay dpy, EGLSurface surface);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLGETCONFIGATTRIBPROC)(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint* value);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLGETCONFIGSPROC)(EGLDisplay dpy, EGLConfig* configs, EGLint config_size, EGLint* num_config);
typedef EGLDisplay(EGLAPIENTRYP PFNEGLGETCURRENTDISPLAYPROC)(void);
typedef EGLSurface(EGLAPIENTRYP PFNEGLGETCURRENTSURFACEPROC)(EGLint readdraw);
typedef EGLDisplay(EGLAPIENTRYP PFNEGLGETDISPLAYPROC)(EGLNativeDisplayType display_id);
typedef EGLint(EGLAPIENTRYP PFNEGLGETERRORPROC)(void);
typedef __eglMustCastToProperFunctionPointerType(EGLAPIENTRYP PFNEGLGETPROCADDRESSPROC)(const char* procname);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLINITIALIZEPROC)(EGLDisplay dpy, EGLint* major, EGLint* minor);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLMAKECURRENTPROC)(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLQUERYCONTEXTPROC)(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint* value);
typedef const char*(EGLAPIENTRYP PFNEGLQUERYSTRINGPROC)(EGLDisplay dpy, EGLint name);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLQUERYSURFACEPROC)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint* value);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLSWAPBUFFERSPROC)(EGLDisplay dpy, EGLSurface surface);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLTERMINATEPROC)(EGLDisplay dpy);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLWAITGLPROC)(void);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLWAITNATIVEPROC)(EGLint engine);

// EGL 1.1
typedef EGLBoolean(EGLAPIENTRYP PFNEGLBINDTEXIMAGEPROC)(EGLDisplay dpy, EGLSurface surface, EGLint buffer);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLRELEASETEXIMAGEPROC)(EGLDisplay dpy, EGLSurface surface, EGLint buffer);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLSURFACEATTRIBPROC)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLSWAPINTERVALPROC)(EGLDisplay dpy, EGLint interval);

// EGL 1.2
typedef EGLBoolean(EGLAPIENTRYP PFNEGLBINDAPIPROC)(EGLenum api);
typedef EGLenum(EGLAPIENTRYP PFNEGLQUERYAPIPROC)(void);
typedef EGLSurface(EGLAPIENTRYP PFNEGLCREATEPBUFFERFROMCLIENTBUFFERPROC)(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint* attrib_list);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLRELEASETHREADPROC)(void);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLWAITCLIENTPROC)(void);

// EGL 1.4
typedef EGLContext(EGLAPIENTRYP PFNEGLGETCURRENTCONTEXTPROC)(void);

// EGL 1.5
typedef EGLSync(EGLAPIENTRYP PFNEGLCREATESYNCPROC)(EGLDisplay dpy, EGLenum type, const EGLAttrib* attrib_list);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLDESTROYSYNCPROC)(EGLDisplay dpy, EGLSync sync);
typedef EGLint(EGLAPIENTRYP PFNEGLCLIENTWAITSYNCPROC)(EGLDisplay dpy, EGLSync sync, EGLint flags, EGLTime timeout);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLGETSYNCATTRIBPROC)(EGLDisplay dpy, EGLSync sync, EGLint attribute, EGLAttrib* value);
typedef EGLImage(EGLAPIENTRYP PFNEGLCREATEIMAGEPROC)(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLAttrib* attrib_list);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLDESTROYIMAGEPROC)(EGLDisplay dpy, EGLImage image);
typedef EGLDisplay(EGLAPIENTRYP PFNEGLGETPLATFORMDISPLAYPROC)(EGLenum platform, void* native_display, const EGLAttrib* attrib_list);
typedef EGLSurface(EGLAPIENTRYP PFNEGLCREATEPLATFORMWINDOWSURFACEPROC)(EGLDisplay dpy, EGLConfig config, void* native_window, const EGLAttrib* attrib_list);
typedef EGLSurface(EGLAPIENTRYP PFNEGLCREATEPLATFORMPIXMAPSURFACEPROC)(EGLDisplay dpy, EGLConfig config, void* native_pixmap, const EGLAttrib* attrib_list);
typedef EGLBoolean(EGLAPIENTRYP PFNEGLWAITSYNCPROC)(EGLDisplay dpy, EGLSync sync, EGLint flags);

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

    PFNEGLCREATEIMAGEKHRPROC CreateImageKHR;
    PFNEGLDESTROYIMAGEKHRPROC DestroyImageKHR;

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
    PFNEGLGETNATIVECLIENTBUFFERANDROIDPROC GetNativeClientBufferANDROID;

    // EGL_ANDROID_native_fence_sync
    PFNEGLDUPNATIVEFENCEFDANDROIDPROC DupNativeFenceFDANDROID;

private:
    uint32_t m_majorVersion{ 0 };
    uint32_t m_minorVersion{ 0 };
};

} // namespace june