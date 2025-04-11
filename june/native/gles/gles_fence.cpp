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
}

void GLESFence::begin()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    for (auto& waitSync : m_waitSync)
    {
        EGLSyncKHR eglSyncKHR = waitSync.second;

        if (eglSyncKHR == EGL_NO_SYNC_KHR)
        {
            spdlog::debug("No wait. because EGLSyncKHR is EGL_NO_SYNC_KHR");
            continue;
        }

        auto context = static_cast<GLESApiContext*>(m_context);
        const auto& eglAPI = context->eglAPI;

        // eglAPI.WaitSyncKHR(context->getEGLDisplay(), eglSyncKHR, 0);
        eglAPI.ClientWaitSyncKHR(context->getEGLDisplay(), eglSyncKHR, 0, EGL_FOREVER_KHR);
    }
}

void GLESFence::end()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    auto context = static_cast<GLESApiContext*>(m_context);
    const auto& eglAPI = context->eglAPI;

    if (m_signalSync != EGL_NO_SYNC_KHR)
    {
        EGLint value;
        eglAPI.GetSyncAttribKHR(context->getEGLDisplay(), m_signalSync, EGL_SYNC_STATUS_KHR, &value);
        spdlog::trace("Current EGLSyncKHR status before creating a new EGLSyncKHR: {}", value);
        // EGL_SIGNALED_KHR       12530
        // EGL_UNSIGNALED_KHR     12531

        if (value == EGL_UNSIGNALED_KHR)
        {
            eglAPI.ClientWaitSyncKHR(context->getEGLDisplay(), m_signalSync, 0, EGL_FOREVER_KHR);
            eglAPI.DestroySyncKHR(context->getEGLDisplay(), m_signalSync);
            m_signalSync = EGL_NO_SYNC_KHR;
            spdlog::trace("Wait and destroy the EGLSyncKHR for creating a new EGLSyncKHR");
        }
    }

    m_signalSync = eglAPI.CreateSyncKHR(context->getEGLDisplay(), EGL_SYNC_NATIVE_FENCE_ANDROID, nullptr);
    if (m_signalSync == EGL_NO_SYNC_KHR)
    {
        spdlog::error("Failed to create a EGLSyncKHR");
        return;
    }

    EGLint value;
    eglAPI.GetSyncAttribKHR(context->getEGLDisplay(), m_signalSync, EGL_SYNC_STATUS_KHR, &value);

    // EGL_SIGNALED_KHR       12530
    // EGL_UNSIGNALED_KHR     12531
    spdlog::trace("Created EGLSyncKHR status: {}", value);

    signal();
}

EGLSyncKHR GLESFence::getEGLSyncKHR() const
{
    return m_sync;
}

int GLESFence::getFenceFd() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    auto context = static_cast<GLESApiContext*>(m_context);
    const auto& eglAPI = context->eglAPI;

    auto fenceFd = static_cast<int>(eglAPI.DupNativeFenceFDANDROID(context->getEGLDisplay(), m_signalSync));
    spdlog::trace("Created EGLSyncKHR fenceFd: {}", fenceFd);
    if (fenceFd == -1)
    {
        spdlog::error("Failed to duplicate EGLSyncKHR fenceFd");
    }

    return fenceFd;
}

void GLESFence::updated(Fence* fence)
{
    EGLSyncKHR eglSyncKHR = EGL_NO_SYNC_KHR;

    switch (fence->getType())
    {
    case FenceType::kFenceType_GLES: {
        eglSyncKHR = static_cast<GLESFence*>(fence)->getEGLSyncKHR();
    }
    case FenceType::kFenceType_Vulkan: {
        auto fenceFD = static_cast<VulkanFence*>(fence)->getFenceFd();

        EGLint attribs[] = {
            EGL_SYNC_NATIVE_FENCE_ANDROID, fenceFD,
            EGL_NONE
        };

        auto context = static_cast<GLESApiContext*>(m_context);
        const auto& eglAPI = context->eglAPI;

        eglSyncKHR = eglAPI.CreateSyncKHR(context->getEGLDisplay(), EGL_SYNC_NATIVE_FENCE_ANDROID, attribs);
    }
    break;
    default:
        spdlog::error("Unsupported fence type: {}", static_cast<uint32_t>(fence->getType()));
        break;
    }

    if (eglSyncKHR == EGL_NO_SYNC_KHR)
    {
        spdlog::error("Failed to create EGLSyncKHR");
    }

    m_waitSync[fence] = eglSyncKHR;
}

} // namespace june