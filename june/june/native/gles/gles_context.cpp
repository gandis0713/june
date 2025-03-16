#include "gles_context.h"

#include "gles_buffer.h"
#include "gles_texture.h"
#include "june/common/assert.h"

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

// memory
const char kExtensionNameKhrExternalMemory[] = "VK_KHR_external_memory";

namespace june
{

Context* GLESContext::create(Instance* instance, JuneGLESApiContextDescriptor const* descriptor)
{
    return new GLESContext(instance, descriptor);
}

GLESContext::GLESContext(Instance* instance, JuneGLESApiContextDescriptor const* descriptor)
    : m_instance(instance)
    , m_eglContext(descriptor->context)
    , m_eglDisplay(descriptor->display)
{
    // #if defined(__ANDROID__) || defined(ANDROID)
    //     const char glesLibraryName[] = "libgles.so";
    // #elif defined(__linux__)
    //     const char glesLibraryName[] = "libgles.so.1";
    // #elif defined(__APPLE__)
    //     const char glesLibraryName[] = "libgles.dylib";
    // #elif defined(WIN32)
    //     const char glesLibraryName[] = "gles-1.dll";
    // #endif

    // if (!m_glesLib.open(glesLibraryName))
    // {
    //     throw std::runtime_error(fmt::format("Failed to open gles library: {}", glesLibraryName));
    // }

    // if (!vkAPI.loadInstanceProcs(&m_glesLib))
    // {
    //     throw std::runtime_error(fmt::format("Failed to load instance prosc in gles library: {}", glesLibraryName));
    // }
}

GLESContext::~GLESContext()
{
    // do not destroy instance for gles.
}

Buffer* GLESContext::createBuffer(JuneBufferDescriptor const* descriptor)
{
    return GLESBuffer::create(this, descriptor);
}

Texture* GLESContext::createTexture(JuneTextureDescriptor const* descriptor)
{
    return GLESTexture::create(this, descriptor);
}

Instance* GLESContext::getInstance() const
{
    return m_instance;
}

} // namespace june
