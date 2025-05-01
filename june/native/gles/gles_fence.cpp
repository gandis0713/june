#include "gles_fence.h"

#include "gles_api_context.h"
#include "june/native/vulkan/vulkan_fence.h"

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
    m_type = FenceType::kFenceType_GLES;

    refresh();
}

void GLESFence::refresh()
{
    waitEGLSyncKHR();
    createEGLSyncKHR();
    createFd();
}

EGLSyncKHR GLESFence::getEGLSyncKHR() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_signalSync;
}

int GLESFence::getFd() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_signalFd;
}

void GLESFence::waitEGLSyncKHR()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    auto context = static_cast<GLESApiContext*>(m_context);
    const auto& eglAPI = context->eglAPI;

    if (m_signalSync != EGL_NO_SYNC_KHR)
    {
        EGLint value;
        eglAPI.GetSyncAttribKHR(context->getEGLDisplay(), m_signalSync, EGL_SYNC_STATUS_KHR, &value);
        spdlog::trace("Current EGLSyncKHR status before waiting: {}", value);
        // EGL_SIGNALED_KHR       12530
        // EGL_UNSIGNALED_KHR     12531

        if (value == EGL_UNSIGNALED_KHR)
        {
            eglAPI.ClientWaitSyncKHR(context->getEGLDisplay(), m_signalSync, 0, EGL_FOREVER_KHR);
        }

        eglAPI.DestroySyncKHR(context->getEGLDisplay(), m_signalSync);
        m_signalSync = EGL_NO_SYNC_KHR;
        m_signalFd = -1;
        spdlog::trace("Wait and destroy the EGLSyncKHR");
    }
}

void GLESFence::createEGLSyncKHR()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    auto context = static_cast<GLESApiContext*>(m_context);
    const auto& eglAPI = context->eglAPI;

    if (m_signalSync != EGL_NO_SYNC_KHR)
    {
        spdlog::debug("EGLSyncKHR already created.");
        return;
    }

    m_signalSync = eglAPI.CreateSyncKHR(context->getEGLDisplay(), EGL_SYNC_NATIVE_FENCE_ANDROID, nullptr);
    if (m_signalSync == EGL_NO_SYNC_KHR)
    {
        spdlog::error("Failed to create a EGLSyncKHR");
        return;
    }
}

void GLESFence::createFd()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_signalSync == EGL_NO_SYNC_KHR)
    {
        spdlog::error("EGLSyncKHR is not created yet.");
        return;
    }

    auto context = static_cast<GLESApiContext*>(m_context);
    const auto& eglAPI = context->eglAPI;

    if (m_signalFd != -1)
    {
        spdlog::debug("No need to create EGLSyncKHR. because already created.");
        return;
    }

    m_signalFd = static_cast<int>(eglAPI.DupNativeFenceFDANDROID(context->getEGLDisplay(),
                                                                 m_signalSync));
    spdlog::trace("Created EGLSyncKHR fenceFd: {}", m_signalFd);
    if (m_signalFd == -1)
    {
        spdlog::error("Failed to duplicate EGLSyncKHR fenceFd");
    }
}
} // namespace june