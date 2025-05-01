#include "gles_api_context.h"

#include "gles_fence.h"
#include "june/common/assert.h"
#include "june/native/vulkan/vulkan_fence.h"

#include "june/native/shared_memory.h"

#if __has_include("gles_ahardwarebuffer_eglimage.h")
#include "gles_ahardwarebuffer_eglimage.h"
#endif

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace june
{

ApiContext* GLESApiContext::create(Instance* instance, JuneGLESApiContextDescriptor const* descriptor)
{
    return new GLESApiContext(instance, descriptor);
}

GLESApiContext::GLESApiContext(Instance* instance, JuneGLESApiContextDescriptor const* descriptor)
    : m_instance(instance)
    , m_context(static_cast<EGLContext>(descriptor->context))
    , m_display(static_cast<EGLDisplay>(descriptor->display))
{
#if defined(__ANDROID__) || defined(ANDROID) || defined(__linux__)
    const char glesLibraryName[] = "libEGL.so";
#elif defined(__APPLE__)
    const char glesLibraryName[] = "libEGL.dylib";
#elif defined(WIN32)
    const char glesLibraryName[] = "libEGL-1.dll";
#endif

    bool loaded = m_glesLib.open(glesLibraryName);
    if (!eglAPI.loadClientProcs(loaded ? &m_glesLib : nullptr))
    {
        throw std::runtime_error(fmt::format("Failed to load GLES library: {}", glesLibraryName));
    }

    if (!eglAPI.loadDisplayProcs(m_display))
    {
        spdlog::warn("Failed to load GLES display procs");
    }
}

GLESApiContext::~GLESApiContext()
{
    if (m_glesLib.isValid())
    {
        m_glesLib.close();
    }
}

void GLESApiContext::createResource(JuneResourceDescriptor const* descriptor)
{
    auto sharedMemory = reinterpret_cast<SharedMemory*>(descriptor->sharedMemory);
    auto rawMemory = sharedMemory->getRawMemory();

    switch (rawMemory->getType())
    {
#if defined(__ANDROID__) || defined(ANDROID)
    case RawMemoryType::kAHardwareBuffer: {
        GLESAHardwareBufferEGLImage::create(this, descriptor);
        return;
    }
#endif
    default:
        break;
    }
}

Fence* GLESApiContext::createFence(JuneFenceDescriptor const* descriptor)
{
    return GLESFence::create(this, descriptor);
}

void GLESApiContext::beginMemoryAccess(JuneApiContextBeginMemoryAccessDescriptor const* descriptor)
{
    reinterpret_cast<SharedMemory*>(descriptor->sharedMemory)->lock(this);

    const auto fenceCount = descriptor->waitSyncInfo->fenceCount;
    std::vector<EGLSyncKHR> eglSyncs(fenceCount);

    for (auto i = 0; i < fenceCount; ++i)
    {
        auto fence = reinterpret_cast<Fence*>(descriptor->waitSyncInfo->fences[i]);
        switch (fence->getType())
        {
        case FenceType::kFenceType_GLES:
            eglSyncs[i] = static_cast<GLESFence*>(fence)->getEGLSyncKHR();
            break;
        case FenceType::kFenceType_Vulkan:
            eglSyncs[i] = createEGLSyncKHR(static_cast<VulkanFence*>(fence)->getFd());
            break;
        }
    }

    JuneChainedStruct* current = descriptor->exportedSyncObject->nextInChain;
    while (current)
    {
        switch (current->sType)
        {
        case JuneSType_SharedMemoryExportedEGLSyncKHRSyncObject: {
            auto eglSyncObject = reinterpret_cast<JuneSharedMemoryExportedEGLSyncKHRSyncObject*>(current);
            eglSyncObject->eglSyncCount = static_cast<uint32_t>(eglSyncs.size());
            eglSyncObject->eglSyncs = reinterpret_cast<void*>(eglSyncs.data());
            break;
        }
        default:
            break;
        }

        current = current->next;
    }
}

void GLESApiContext::endMemoryAccess(JuneApiContextEndMemoryAccessDescriptor const* descriptor)
{
    const auto fenceCount = descriptor->signalSyncInfo->fenceCount;
    std::vector<EGLSyncKHR> eglSyncs(fenceCount);

    for (auto i = 0; i < fenceCount; ++i)
    {
        auto fence = reinterpret_cast<Fence*>(descriptor->signalSyncInfo->fences[i]);
        switch (fence->getType())
        {
        case FenceType::kFenceType_GLES:
            static_cast<GLESFence*>(fence)->refresh();
            break;
        case FenceType::kFenceType_Vulkan:
            eglSyncs[i] = createEGLSyncKHR(static_cast<VulkanFence*>(fence)->getFd());
            break;
        }
    }

    reinterpret_cast<SharedMemory*>(descriptor->sharedMemory)->unlock(this);
}

Instance* GLESApiContext::getInstance() const
{
    return m_instance;
}

JuneApiType GLESApiContext::getApiType() const
{
    return JuneApiType_GLES;
}

EGLContext GLESApiContext::getEGLContext() const
{
    return m_context;
}

EGLDisplay GLESApiContext::getEGLDisplay() const
{
    return m_display;
}

EGLSyncKHR GLESApiContext::createEGLSyncKHR(const int fd)
{
    EGLint attribs[] = {
        EGL_SYNC_NATIVE_FENCE_ANDROID, fd,
        EGL_NONE
    };

    auto context = static_cast<GLESApiContext*>(m_context);
    const auto& eglAPI = context->eglAPI;

    auto eglSyncKHR = eglAPI.CreateSyncKHR(context->getEGLDisplay(), EGL_SYNC_NATIVE_FENCE_ANDROID, attribs);
    if (eglSyncKHR == EGL_NO_SYNC_KHR)
    {
        spdlog::error("Failed to create EGLSyncKHR");
        return EGL_NO_SYNC_KHR;
    }

    return eglSyncKHR;
}

} // namespace june
