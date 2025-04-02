#include "gles_fence.h"

#include "gles_api_context.h"

#include <spdlog/spdlog.h>

namespace june
{

GLESFence* GLESFence::create(GLESApiContext* context, JuneFenceDescriptor const* descriptor)
{
    return new GLESFence(context, descriptor);
}

GLESFence::GLESFence(GLESApiContext* context, JuneFenceDescriptor const* descriptor)
    : Fence(context, descriptor)
{
}

void GLESFence::begin()
{
    std::lock_guard<std::mutex> lock(m_mutex);
}

void GLESFence::end()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    auto context = static_cast<GLESApiContext*>(m_context);
    const auto& eglAPI = context->eglAPI;

    if (m_sync != EGL_NO_SYNC_KHR)
    {
        EGLint value;
        eglAPI.GetSyncAttribKHR(context->getEGLDisplay(), m_sync, EGL_SYNC_STATUS_KHR, &value);
        spdlog::trace("Current EGLSyncKHR status before creating a new EGLSyncKHR: {}", value);
        // EGL_SIGNALED_KHR       12530
        // EGL_UNSIGNALED_KHR     12531

        if (value == EGL_UNSIGNALED_KHR)
        {
            eglAPI.ClientWaitSyncKHR(context->getEGLDisplay(), m_sync, 0, EGL_FOREVER_KHR);
            eglAPI.DestroySyncKHR(context->getEGLDisplay(), m_sync);
            m_sync = EGL_NO_SYNC_KHR;
            spdlog::trace("Wait and destroy the EGLSyncKHR for creating a new EGLSyncKHR");
        }
    }

    m_sync = eglAPI.CreateSyncKHR(context->getEGLDisplay(), EGL_SYNC_NATIVE_FENCE_ANDROID, nullptr);
    if (m_sync == EGL_NO_SYNC_KHR)
    {
        spdlog::error("Failed to create a EGLSyncKHR");
        return;
    }

    EGLint value;
    eglAPI.GetSyncAttribKHR(context->getEGLDisplay(), m_sync, EGL_SYNC_STATUS_KHR, &value);
    spdlog::trace("Created EGLSyncKHR status: {}", value);
}

void GLESFence::wait()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_sync == EGL_NO_SYNC_KHR)
    {
        spdlog::trace("No wait. because EGLSyncKHR is null");
        return;
    }
    auto context = static_cast<GLESApiContext*>(m_context);
    const auto& eglAPI = context->eglAPI;

    eglAPI.WaitSyncKHR(context->getEGLDisplay(), m_sync, 0);
    eglAPI.DestroySyncKHR(context->getEGLDisplay(), m_sync);
    m_sync = EGL_NO_SYNC_KHR;
    spdlog::trace("Wait and destroy the EGLSyncKHR");
}

} // namespace june