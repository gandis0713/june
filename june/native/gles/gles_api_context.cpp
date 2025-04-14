#include "gles_api_context.h"

#include "gles_fence.h"
#include "gles_resource.h"
#include "june/common/assert.h"

#include "june/native/shared_memory.h"

#if __has_include("gles_ahardwarebuffer_resource.h")
#include "gles_ahardwarebuffer_resource.h"
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

Resource* GLESApiContext::createResource(JuneResourceDescriptor const* descriptor)
{
    auto sharedMemory = reinterpret_cast<SharedMemory*>(descriptor->sharedMemory);
    auto rawMemory = sharedMemory->getRawMemory();

    switch (rawMemory->getType())
    {
#if defined(__ANDROID__) || defined(ANDROID)
    case RawMemoryType::kAHardwareBuffer: {
        return GLESAHardwareBufferResource::create(this, descriptor);
    }
    break;
#endif

    default:
        return nullptr;
    }

    return nullptr;
}

Fence* GLESApiContext::createFence(JuneFenceDescriptor const* descriptor)
{
    return GLESFence::create(this, descriptor);
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

} // namespace june
