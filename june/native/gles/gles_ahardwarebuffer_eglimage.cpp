#include "gles_ahardwarebuffer_eglimage.h"

#include "gles_api.h"
#include "gles_api_context.h"
#include "gles_fence.h"
#include "june/memory/ahardwarebuffer_memory.h"
#include "june/native/shared_memory.h"

#include <spdlog/spdlog.h>

namespace june
{

int GLESAHardwareBufferEGLImage::create(GLESApiContext* context, JuneResourceDescriptor const* descriptor)
{
    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        switch (current->sType)
        {
        case JuneSType_EGLImageResourceDescriptor: {
            EGLImageKHR eglImage = EGL_NO_IMAGE_KHR;
            EGLClientBuffer eglClientBuffer = nullptr;

            {
                auto sharedMemory = reinterpret_cast<SharedMemory*>(descriptor->sharedMemory);
                auto ahbMemory = static_cast<AHardwareBufferMemory*>(sharedMemory->getRawMemory());

                auto glesApiContext = reinterpret_cast<GLESApiContext*>(context);
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

                eglClientBuffer = eglAPI.GetNativeClientBufferANDROID(ahbMemory->getAHardwareBuffer());
                if (eglClientBuffer == nullptr)
                {
                    spdlog::error("Failed to get EGLClientBuffer from AHardwareBuffer.");
                    return -1;
                }
            }

            {
                EGLint imageAttribs[] = {
                        EGL_IMAGE_PRESERVED_KHR, EGL_TRUE,
                        EGL_NONE
                };

                auto glesApiContext = reinterpret_cast<GLESApiContext*>(context);
                const auto& eglAPI = glesApiContext->eglAPI;
                eglImage = eglAPI.CreateImageKHR(glesApiContext->getEGLDisplay(), EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID,
                                                 eglClientBuffer, imageAttribs);
                if (eglImage == EGL_NO_IMAGE_KHR)
                {
                    spdlog::error("Failed to create EGLImage from AHardwareBuffer.");
                    return -1;
                }

                const auto* imageDesc = reinterpret_cast<const JuneResourceEGLImageDescriptor*>(current);
                imageDesc->eglImageResultInfo->eglClientBuffer = eglClientBuffer;
                imageDesc->eglImageResultInfo->eglImage = eglImage;

                return 0;
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