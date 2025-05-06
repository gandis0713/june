#include "gles_fence.h"

#include "gles_context.h"
#include "june/native/vulkan/vulkan_fence.h"

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

    refresh();
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

        // if (value == EGL_UNSIGNALED_KHR)
        //     eglAPI.ClientWaitSyncKHR(context->getEGLDisplay(), m_signalSync, 0, EGL_FOREVER_KHR);

        eglAPI.DestroySyncKHR(context->getEGLDisplay(), m_signalSync);
        m_signalSync = EGL_NO_SYNC_KHR;
        spdlog::trace("Wait and destroy the EGLSync");
    }

    // create
    m_signalSync = eglAPI.CreateSyncKHR(context->getEGLDisplay(), EGL_SYNC_NATIVE_FENCE_ANDROID, nullptr);
    if (m_signalSync == EGL_NO_SYNC_KHR)
    {
        spdlog::error("Failed to create a EGLSync");
        return;
    }
}

// void GLESFence::wait(JuneFenceWaitDescriptor const* descriptor)
// {
//     std::lock_guard<std::mutex> lock(m_mutex);

//     if (m_signalSync == EGL_NO_SYNC_KHR)
//     {
//         spdlog::error("EGLSync is not created yet.");
//         return;
//     }

//     auto context = static_cast<GLESContext*>(m_context);
//     const auto& eglAPI = context->eglAPI;

//     EGLint value;
//     eglAPI.GetSyncAttribKHR(context->getEGLDisplay(), m_signalSync, EGL_SYNC_STATUS_KHR, &value);
//     spdlog::trace("Current EGLSync status before waiting: {}", value);
//     // EGL_SIGNALED_KHR       12530
//     // EGL_UNSIGNALED_KHR     12531

//     if (value == EGL_UNSIGNALED_KHR)
//         eglAPI.ClientWaitSyncKHR(context->getEGLDisplay(), m_signalSync, 0, EGL_FOREVER_KHR);

//     eglAPI.DestroySyncKHR(context->getEGLDisplay(), m_signalSync);
//     m_signalSync = EGL_NO_SYNC_KHR;
// }

void GLESFence::exportFence(JuneFenceExportDescriptor const* descriptor)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_signalSync == EGL_NO_SYNC_KHR)
    {
        spdlog::error("EGLSync is not created yet.");
        return;
    }

    auto context = static_cast<GLESContext*>(m_context);
    const auto& eglAPI = context->eglAPI;

    JuneChainedStruct* current = descriptor->nextInChain;

    while (current)
    {
        switch (current->sType)
        {
        case JuneSType_FenceEGLSyncExportDescriptor: {

            int syncFD = static_cast<int>(eglAPI.DupNativeFenceFDANDROID(context->getEGLDisplay(),
                                                                         m_signalSync));
            spdlog::trace("Duplicated sync FD: {}", syncFD);
            if (syncFD == -1)
            {
                spdlog::error("Failed to duplicate sync FD from EGLSync");
                return;
            }

            // create
            EGLint attribs[] = {
                EGL_SYNC_NATIVE_FENCE_FD_ANDROID, syncFD,
                EGL_NONE
            };

            auto eglSync = eglAPI.CreateSyncKHR(context->getEGLDisplay(), EGL_SYNC_NATIVE_FENCE_ANDROID, attribs);
            if (eglSync == EGL_NO_SYNC_KHR)
            {
                spdlog::error("Failed to create a EGLSync for exporting.");
                return;
            }

            EGLint value;
            eglAPI.GetSyncAttribKHR(context->getEGLDisplay(), eglSync, EGL_SYNC_STATUS_KHR, &value);
            spdlog::trace("Current EGLSync status for exporting: {}", value);
            // EGL_SIGNALED_KHR       12530
            // EGL_UNSIGNALED_KHR     12531

            auto eglSyncExportDescriptor = reinterpret_cast<JuneFenceEGLSyncExportDescriptor*>(current);
            eglSyncExportDescriptor->eglSync = eglSync;
            break;
        }
        default:
            break;
        }

        current = current->next;
    }
}

void GLESFence::refresh()
{
}

int GLESFence::getFd() const
{
    return -1;
}

} // namespace june