#include "gles_ahardwarebuffer_resource.h"

#include "gles_api.h"
#include "gles_api_context.h"
#include "gles_fence.h"
#include "june/memory/ahardwarebuffer_memory.h"
#include "june/native/shared_memory.h"

#include <spdlog/spdlog.h>

namespace june
{

GLESAHardwareBufferResource* GLESAHardwareBufferResource::create(GLESApiContext* context, JuneResourceDescriptor const* descriptor)
{
    auto resource = new GLESAHardwareBufferResource(context, descriptor);
    resource->initialize();

    return resource;
}

GLESAHardwareBufferResource::GLESAHardwareBufferResource(GLESApiContext* context, JuneResourceDescriptor const* descriptor)
    : GLESResource(context, descriptor)
{
}

void GLESAHardwareBufferResource::beginAccess(JuneResourceBeginAccessDescriptor const* descriptor)
{
    m_accessMutex.lock();

    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        current = current->next;
    }

    m_fence->begin();
}

void GLESAHardwareBufferResource::endAccess(JuneResourceEndAccessDescriptor const* descriptor)
{
    m_fence->end();

    signal();
}

void* GLESAHardwareBufferResource::getResource(JuneGetResourceDescriptor const* descriptor)
{
    return static_cast<void*>(m_eglImage);
}

int32_t GLESAHardwareBufferResource::initialize()
{
    createEGLClientBuffer();
    createEGLImageKHR();

    auto glesApiContext = reinterpret_cast<GLESApiContext*>(m_context);
    const auto& eglAPI = glesApiContext->eglAPI;

    JuneFenceDescriptor fenceDescriptor{};
    m_fence = GLESFence::create(static_cast<GLESApiContext*>(m_context), &fenceDescriptor);

    return 0;
}

int32_t GLESAHardwareBufferResource::createEGLClientBuffer()
{
    if (m_clientBuffer != nullptr)
    {
        spdlog::error("EGLClientBuffer already created.");
        return -1;
    }

    auto sharedMemory = reinterpret_cast<SharedMemory*>(m_descriptor.sharedMemory);
    auto ahbMemory = static_cast<AHardwareBufferMemory*>(sharedMemory->getRawMemory());

    auto glesApiContext = reinterpret_cast<GLESApiContext*>(m_context);
    const auto& eglAPI = glesApiContext->eglAPI;

    if (!eglAPI.GetNativeClientBufferANDROID)
    {
        spdlog::error("eglGetNativeClientBufferANDROID function pointer acquisition failed.");
        return -1;
    }

    if (!eglAPI.DupNativeFenceFDANDROID)
    {
        spdlog::error("eglDupNativeFenceFDANDROID function pointer acquisition failed.");
        return -1;
    }

    if (!eglAPI.CreateImageKHR)
    {
        spdlog::error("eglCreateImageKHR function pointer acquisition failed.");
        return -1;
    }

    if (!eglAPI.DestroyImageKHR)
    {
        spdlog::error("eglDestroyImageKHR function pointer acquisition failed.");
        return -1;
    }

    m_clientBuffer = eglAPI.GetNativeClientBufferANDROID(ahbMemory->getAHardwareBuffer());
    if (m_clientBuffer == nullptr)
    {
        spdlog::error("Failed to get EGLClientBuffer from AHardwareBuffer.");
        return -1;
    }

    spdlog::trace("Successfully created EGLClientBuffer from AHardwareBuffer.");

    return 0;
}

int32_t GLESAHardwareBufferResource::createEGLImageKHR()
{
    if (m_eglImage != EGL_NO_IMAGE_KHR)
    {
        spdlog::error("EGLImage already created.");
        return -1;
    }

    if (m_clientBuffer == nullptr)
    {
        spdlog::error("EGLClientBuffer is null.");
        return -1;
    }

    const JuneChainedStruct* current = m_descriptor.nextInChain;
    while (current)
    {
        switch (current->sType)
        {
        case JuneStype_EGLImageResourceDescriptor: {
            EGLint imageAttribs[] = {
                EGL_IMAGE_PRESERVED_KHR, EGL_TRUE,
                EGL_NONE
            };

            auto glesApiContext = reinterpret_cast<GLESApiContext*>(m_context);
            const auto& eglAPI = glesApiContext->eglAPI;
            m_eglImage = eglAPI.CreateImageKHR(glesApiContext->getEGLDisplay(), EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID,
                                               m_clientBuffer, imageAttribs);
            if (m_eglImage == EGL_NO_IMAGE_KHR)
            {
                spdlog::error("Failed to create EGLImage from AHardwareBuffer.");
                return -1;
            }
        }
        break;
        default:
            spdlog::error("Unsupported resource type: {}", static_cast<uint32_t>(current->sType));
            return -1;
        }

        current = current->next;
    }

    return 0;
}

} // namespace june