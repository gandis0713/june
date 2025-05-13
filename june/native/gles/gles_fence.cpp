#include "gles_fence.h"

#include "gles_context.h"
#include "june/native/vulkan/vulkan_fence.h"
#include <GLES/gl.h>

#include <spdlog/spdlog.h>

namespace june
{

GLESFence* GLESFence::create(GLESContext* context, JuneFenceCreateDescriptor const* descriptor)
{
    return new GLESFence(context, descriptor);
}

GLESFence::GLESFence(GLESContext* context, JuneFenceCreateDescriptor const* descriptor)
    : Fence(context, descriptor)
{
    m_type = FenceType::kFenceType_SyncFD;
}

void GLESFence::reset(JuneFenceResetDescriptor const* descriptor)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    auto context = static_cast<GLESContext*>(m_context);
    const auto& eglAPI = context->eglAPI;

    // destroy
    if (m_signalSync != EGL_NO_SYNC_KHR)
    {
        EGLint value;
        eglAPI.GetSyncAttribKHR(context->getEGLDisplay(), m_signalSync, EGL_SYNC_STATUS_KHR, &value);
        spdlog::trace("Current EGLSync status before waiting: {}", value);
        // EGL_SIGNALED_KHR       12530
        // EGL_UNSIGNALED_KHR     12531

        if (value == EGL_UNSIGNALED_KHR)
            eglAPI.ClientWaitSyncKHR(context->getEGLDisplay(), m_signalSync, 0, EGL_FOREVER_KHR);

        spdlog::trace("{} Destroy EGLSync for new one. {:p}", getName(), m_signalSync);
        eglAPI.DestroySyncKHR(context->getEGLDisplay(), m_signalSync);
        m_signalSync = EGL_NO_SYNC_KHR;
    }

    // create
    EGLint attribs[] = {
        EGL_SYNC_NATIVE_FENCE_FD_ANDROID, EGL_NO_NATIVE_FENCE_FD_ANDROID,
        EGL_NONE
    };
    m_signalSync = eglAPI.CreateSyncKHR(context->getEGLDisplay(), EGL_SYNC_NATIVE_FENCE_ANDROID, attribs);
    if (m_signalSync == EGL_NO_SYNC_KHR)
    {
        spdlog::error("Failed to create a EGLSync");
        return;
    }

    spdlog::trace("Succeed create the EGLSync");

    // flush need to be called to make sure the sync object is created.
    glFlush();
}

int GLESFence::getSyncFD() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_signalSync == EGL_NO_SYNC_KHR)
    {
        spdlog::error("EGLSync is not created yet.");
        return -1;
    }

    auto context = static_cast<GLESContext*>(m_context);
    const auto& eglAPI = context->eglAPI;

    int syncFD = static_cast<int>(eglAPI.DupNativeFenceFDANDROID(context->getEGLDisplay(),
                                                                 m_signalSync));

    if (syncFD == EGL_NO_NATIVE_FENCE_FD_ANDROID)
    {
        spdlog::error("Failed to duplicate sync FD from EGLSync");
    }

    return syncFD;
}

} // namespace june