#include "gles_ahardwarebuffer_api_memory.h"

#include "gles_api.h"
#include "gles_api_context.h"
#include "june/memory/ahardwarebuffer_memory.h"
#include "june/native/shared_memory.h"

#include <spdlog/spdlog.h>

namespace june
{

GLESAHardwareBufferApiMemory* GLESAHardwareBufferApiMemory::create(GLESApiContext* context, JuneApiMemoryDescriptor const* descriptor)
{
    auto apiMemory = new GLESAHardwareBufferApiMemory(context, descriptor);
    apiMemory->initialize();

    return apiMemory;
}

GLESAHardwareBufferApiMemory::GLESAHardwareBufferApiMemory(GLESApiContext* context, JuneApiMemoryDescriptor const* descriptor)
    : GLESApiMemory(context, descriptor)
{
}

void GLESAHardwareBufferApiMemory::beginAccess(JuneApiMemoryBeginAccessDescriptor const* descriptor)
{
    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        current = current->next;
    }
}

void GLESAHardwareBufferApiMemory::endAccess(JuneApiMemoryEndAccessDescriptor const* descriptor)
{
}

void* GLESAHardwareBufferApiMemory::createResource(JuneResourceDescriptor const* descriptor)
{
    EGLint imageAttribs[] = {
        EGL_IMAGE_PRESERVED_KHR, EGL_TRUE,
        EGL_NONE
    };
    const auto& eglAPI = m_context->eglAPI;
    EGLImageKHR eglImage = eglAPI.CreateImageKHR(m_context->getEGLDisplay(), EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID,
                                                 m_clientBuffer, imageAttribs);
    if (eglImage == EGL_NO_IMAGE_KHR)
    {
        spdlog::error("Failed to create EGLImage from AHardwareBuffer.");
        return nullptr;
    }

    return eglImage;
}

int32_t GLESAHardwareBufferApiMemory::initialize()
{
    auto sharedMemory = reinterpret_cast<SharedMemory*>(m_descriptor.sharedMemory);
    auto ahbMemory = static_cast<AHardwareBufferMemory*>(sharedMemory->getRawMemory());

    const auto& eglAPI = m_context->eglAPI;

    if (!eglAPI.GetNativeClientBufferANDROID)
    {
        spdlog::error("eglGetNativeClientBufferANDROID function pointer acquisition failed.");
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

    EGLClientBuffer clientBuffer = eglAPI.GetNativeClientBufferANDROID(ahbMemory->getAHardwareBuffer());
    if (clientBuffer == nullptr)
    {
        spdlog::error("Failed to get EGLClientBuffer from AHardwareBuffer.");
        return -1;
    }

    m_clientBuffer = clientBuffer;
    spdlog::trace("Successfully created EGLClientBuffer from AHardwareBuffer.");

    return 0;
}

} // namespace june