#include "june/june.h"

namespace june
{

extern JuneProc procGetProcAddress(StringView name);
extern JuneInstance procCreateInstance(JuneInstanceDescriptor const* desc);

extern JuneApiContext procInstanceCreateApiContext(JuneInstance instance, JuneApiContextDescriptor const* desc);
extern void procInstanceDestroy(JuneInstance instance);

extern JuneSharedMemory procApiContextCreateSharedMemory(JuneApiContext context, JuneSharedMemoryDescriptor const* descriptor);
extern JuneBuffer procApiContextCreateBuffer(JuneApiContext context, JuneBufferDescriptor const* descriptor);
extern JuneTexture procApiContextCreateTexture(JuneApiContext context, JuneTextureDescriptor const* descriptor);
extern void procApiContextDestroy(JuneApiContext context);

extern void procSharedMemoryBeginAccess(JuneSharedMemory memory, JuneBeginAccessDescriptor const* descriptor);
extern void procSharedMemoryEndAccess(JuneSharedMemory memory, JuneEndAccessDescriptor const* descriptor);
extern void procSharedMemoryDestroy(JuneSharedMemory bufferMemory);

extern JuneFence procBufferCreateFence(JuneBuffer buffer, JuneFenceDescriptor const* descriptor);
extern void* procBufferGetVkBuffer(JuneBuffer buffer);
extern void procBufferDestroy(JuneBuffer buffer);

extern JuneFence procTextureCreateFence(JuneTexture texture, JuneFenceDescriptor const* descriptor);
extern void* procTextureGetVkImage(JuneTexture texture);
extern void procTextureDestroy(JuneTexture texture);

extern void procFenceDestroy(JuneFence fence);

} // namespace june

extern "C"
{
    using namespace june;

    JUNE_EXPORT JuneProc juneGetProcAddress(StringView name)
    {
        return procGetProcAddress(name);
    }

    JUNE_EXPORT JuneInstance juneCreateInstance(JuneInstanceDescriptor const* desc)
    {
        return procCreateInstance(desc);
    }

    JUNE_EXPORT JuneApiContext juneInstanceCreateApiContext(JuneInstance instance, JuneApiContextDescriptor const* desc)
    {
        return procInstanceCreateApiContext(instance, desc);
    }

    JUNE_EXPORT void juneInstanceDestroy(JuneInstance instance)
    {
        procInstanceDestroy(instance);
    }

    JUNE_EXPORT JuneSharedMemory juneApiContextCreateSharedMemory(JuneApiContext context, JuneSharedMemoryDescriptor const* descriptor)
    {
        return procApiContextCreateSharedMemory(context, descriptor);
    }

    JUNE_EXPORT void juneApiContextDestroy(JuneApiContext context)
    {
        procApiContextDestroy(context);
    }

    JUNE_EXPORT JuneBuffer juneApiContextCreateBuffer(JuneApiContext context, JuneBufferDescriptor const* descriptor)
    {
        return procApiContextCreateBuffer(context, descriptor);
    }

    JUNE_EXPORT JuneTexture juneApiContextCreateTexture(JuneApiContext context, JuneTextureDescriptor const* descriptor)
    {
        return procApiContextCreateTexture(context, descriptor);
    }

    JUNE_EXPORT void juneSharedMemoryBeginAccess(JuneSharedMemory memory, JuneBeginAccessDescriptor const* descriptor)
    {
        procSharedMemoryBeginAccess(memory, descriptor);
    }

    JUNE_EXPORT void juneSharedMemoryEndAccess(JuneSharedMemory memory, JuneEndAccessDescriptor const* descriptor)
    {
        procSharedMemoryEndAccess(memory, descriptor);
    }

    JUNE_EXPORT void juneSharedMemoryDestroy(JuneSharedMemory bufferMemory)
    {
        procSharedMemoryDestroy(bufferMemory);
    }

    JUNE_EXPORT JuneFence juneBufferCreateFence(JuneBuffer buffer, JuneFenceDescriptor const* descriptor)
    {
        return procBufferCreateFence(buffer, descriptor);
    }

    JUNE_EXPORT void* juneBufferGetVkBuffer(JuneBuffer buffer)
    {
        return procBufferGetVkBuffer(buffer);
    }

    JUNE_EXPORT void juneBufferDestroy(JuneBuffer buffer)
    {
        procBufferDestroy(buffer);
    }

    JUNE_EXPORT JuneFence juneTextureCreateFence(JuneTexture texture, JuneFenceDescriptor const* descriptor)
    {
        return procTextureCreateFence(texture, descriptor);
    }

    JUNE_EXPORT void* juneTextureGetVkImage(JuneTexture texture)
    {
        return procTextureGetVkImage(texture);
    }

    JUNE_EXPORT void juneTextureDestroy(JuneTexture texture)
    {
        procTextureDestroy(texture);
    }

    JUNE_EXPORT void juneFenceDestroy(JuneFence fence)
    {
        procFenceDestroy(fence);
    }
}