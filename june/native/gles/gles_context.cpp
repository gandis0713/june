#include "gles_context.h"

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

ApiContext* GLESContext::create(Instance* instance, JuneApiContextDescriptor const* descriptor)
{
    return new GLESContext(instance, descriptor);
}

GLESContext::GLESContext(Instance* instance, JuneApiContextDescriptor const* descriptor)
    : ApiContext(instance, descriptor)
{
    // It assumes that the descriptor is valid and has been validated before this point.
    auto glesDescriptor = reinterpret_cast<JuneGLESContextDescriptor const*>(descriptor->nextInChain);

    m_eglContext = (static_cast<EGLContext>(glesDescriptor->context));
    m_eglDisplay = (static_cast<EGLDisplay>(glesDescriptor->display));

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

    if (!eglAPI.loadDisplayProcs(m_eglDisplay))
    {
        spdlog::warn("Failed to load GLES display procs");
    }
}

GLESContext::~GLESContext()
{
    if (m_glesLib.isValid())
    {
        m_glesLib.close();
    }
}

void GLESContext::createResource(JuneResourceCreateDescriptor const* descriptor)
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

Fence* GLESContext::createFence(JuneFenceCreateDescriptor const* descriptor)
{
    return GLESFence::create(this, descriptor);
}

JuneApiType GLESContext::getApiType() const
{
    return JuneApiType_GLES;
}

EGLContext GLESContext::getEGLContext() const
{
    return m_eglContext;
}

EGLDisplay GLESContext::getEGLDisplay() const
{
    return m_eglDisplay;
}

} // namespace june
