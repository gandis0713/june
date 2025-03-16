#include "june/june.h"

namespace june
{

extern JuneProc procGetProcAddress(char const* procName);
extern JuneInstance procCreateInstance(JuneInstanceDescriptor const* desc);
extern JuneApiContext procCreateApiContext(JuneInstance innstance, JuneApiContextDescriptor const* desc);
extern void procDestroyInstance(JuneInstance instance);
extern void procDestroyApiContext(JuneApiContext context);
extern JuneBufferMemory procCreateBufferMemory(JuneApiContext context, JuneBufferMemoryDescriptor const* descriptor);
extern JuneTextureMemory procCreateTextureMemory(JuneApiContext context, JuneTextureMemoryDescriptor const* descriptor);
extern void procDestroyBufferMemory(JuneBufferMemory bufferMemory);
extern void procDestroyTextureMemory(JuneTextureMemory textureMemory);
extern JuneBuffer procCreateBuffer(JuneBufferMemory memory, JuneBufferDescriptor const* descriptor);
extern JuneTexture procCreateTexture(JuneTextureMemory memory, JuneTextureDescriptor const* descriptor);
extern void procDestroyBuffer(JuneBuffer buffer);
extern void procDestroyTexture(JuneTexture texture);

} // namespace june

extern "C"
{
    using namespace june;

    JUNE_EXPORT JuneProc juneGetProcAddress(char const* procName)
    {
        return procGetProcAddress(procName);
    }

    JUNE_EXPORT JuneInstance juneCreateInstance(JuneInstanceDescriptor const* desc)
    {
        return procCreateInstance(desc);
    }

    JUNE_EXPORT JuneApiContext juneCreateApiContext(JuneInstance innstance, JuneApiContextDescriptor const* desc)
    {
        return procCreateApiContext(innstance, desc);
    }

    JUNE_EXPORT void juneDestroyInstance(JuneInstance instance)
    {
        return procDestroyInstance(instance);
    }

    JUNE_EXPORT void juneDestroyApiContext(JuneApiContext context)
    {
        return procDestroyApiContext(context);
    }

    JUNE_EXPORT JuneBufferMemory juneCreateBufferMemory(JuneApiContext context, JuneBufferMemoryDescriptor const* descriptor)
    {
        return procCreateBufferMemory(context, descriptor);
    }

    JUNE_EXPORT JuneTextureMemory juneCreateTextureMemory(JuneApiContext context, JuneTextureMemoryDescriptor const* descriptor)
    {
        return procCreateTextureMemory(context, descriptor);
    }

    JUNE_EXPORT void juneDestroyBufferMemory(JuneBufferMemory bufferMemory)
    {
        return procDestroyBufferMemory(bufferMemory);
    }

    JUNE_EXPORT void juneDestroyTextureMemory(JuneTextureMemory textureMemory)
    {
        return procDestroyTextureMemory(textureMemory);
    }

    JUNE_EXPORT JuneBuffer juneCreateBuffer(JuneBufferMemory memory, JuneBufferDescriptor const* descriptor)
    {
        return procCreateBuffer(memory, descriptor);
    }

    JUNE_EXPORT JuneTexture juneCreateTexture(JuneTextureMemory memory, JuneTextureDescriptor const* descriptor)
    {
        return procCreateTexture(memory, descriptor);
    }

    JUNE_EXPORT void juneDestroyBuffer(JuneBuffer buffer)
    {
        return procDestroyBuffer(buffer);
    }

    JUNE_EXPORT void juneDestroyTexture(JuneTexture texture)
    {
        return procDestroyTexture(texture);
    }
}