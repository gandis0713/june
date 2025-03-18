#include "june/june.h"

namespace june
{

extern JuneProc procGetProcAddress(StringView name);
extern JuneInstance procCreateInstance(JuneInstanceDescriptor const* desc);

extern JuneApiContext procInstanceCreateApiContext(JuneInstance instance, JuneApiContextDescriptor const* desc);
extern void procInstanceDestroy(JuneInstance instance);

extern JuneBufferMemory procApiContextCreateBufferMemory(JuneApiContext context, JuneBufferMemoryDescriptor const* descriptor);
extern JuneTextureMemory procApiContextCreateTextureMemory(JuneApiContext context, JuneTextureMemoryDescriptor const* descriptor);
extern void procApiContextDestroy(JuneApiContext context);

extern JuneBuffer procBufferMemoryCreateBuffer(JuneBufferMemory memory, JuneBufferDescriptor const* descriptor);
extern void procBufferMemoryBeginAccess(JuneBufferMemory memory, JuneBeginBufferAccessDescriptor const* descriptor);
extern void procBufferMemoryEndAccess(JuneBufferMemory memory, JuneEndBufferAccessDescriptor const* descriptor);
extern void procBufferMemoryDestroy(JuneBufferMemory bufferMemory);

extern JuneTexture procTextureMemoryCreateTexture(JuneTextureMemory memory, JuneTextureDescriptor const* descriptor);
extern void procTextureMemoryBeginAccess(JuneTextureMemory memory, JuneBeginTextureAccessDescriptor const* descriptor);
extern void procTextureMemoryEndAccess(JuneTextureMemory memory, JuneEndTextureAccessDescriptor const* descriptor);
extern void procTextureMemoryDestroy(JuneTextureMemory textureMemory);

extern JuneFence procBufferCreateFence(JuneBuffer buffer, JuneFenceDescriptor const* descriptor);
extern void procBufferDestroy(JuneBuffer buffer);

extern JuneFence procTextureCreateFence(JuneTexture texture, JuneFenceDescriptor const* descriptor);
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

    JUNE_EXPORT JuneBufferMemory juneApiContextCreateBufferMemory(JuneApiContext context, JuneBufferMemoryDescriptor const* descriptor)
    {
        return procApiContextCreateBufferMemory(context, descriptor);
    }

    JUNE_EXPORT JuneTextureMemory juneApiContextCreateTextureMemory(JuneApiContext context, JuneTextureMemoryDescriptor const* descriptor)
    {
        return procApiContextCreateTextureMemory(context, descriptor);
    }

    JUNE_EXPORT void juneApiContextDestroy(JuneApiContext context)
    {
        procApiContextDestroy(context);
    }

    JUNE_EXPORT JuneBuffer juneBufferMemoryCreateBuffer(JuneBufferMemory memory, JuneBufferDescriptor const* descriptor)
    {
        return procBufferMemoryCreateBuffer(memory, descriptor);
    }

    JUNE_EXPORT void juneBufferMemoryBeginAccess(JuneBufferMemory memory, JuneBeginBufferAccessDescriptor const* descriptor)
    {
        procBufferMemoryBeginAccess(memory, descriptor);
    }

    JUNE_EXPORT void juneBufferMemoryEndAccess(JuneBufferMemory memory, JuneEndBufferAccessDescriptor const* descriptor)
    {
        procBufferMemoryEndAccess(memory, descriptor);
    }

    JUNE_EXPORT void juneBufferMemoryDestroy(JuneBufferMemory bufferMemory)
    {
        procBufferMemoryDestroy(bufferMemory);
    }

    JUNE_EXPORT JuneTexture juneTextureMemoryCreateTexture(JuneTextureMemory memory, JuneTextureDescriptor const* descriptor)
    {
        return procTextureMemoryCreateTexture(memory, descriptor);
    }

    JUNE_EXPORT void juneTextureMemoryBeginAccess(JuneTextureMemory memory, JuneBeginTextureAccessDescriptor const* descriptor)
    {
        procTextureMemoryBeginAccess(memory, descriptor);
    }

    JUNE_EXPORT void juneTextureMemoryEndAccess(JuneTextureMemory memory, JuneEndTextureAccessDescriptor const* descriptor)
    {
        procTextureMemoryEndAccess(memory, descriptor);
    }

    JUNE_EXPORT void juneTextureMemoryDestroy(JuneTextureMemory textureMemory)
    {
        procTextureMemoryDestroy(textureMemory);
    }

    JUNE_EXPORT JuneFence juneBufferCreateFence(JuneBuffer buffer, JuneFenceDescriptor const* descriptor)
    {
        return procBufferCreateFence(buffer, descriptor);
    }

    JUNE_EXPORT void juneBufferDestroy(JuneBuffer buffer)
    {
        procBufferDestroy(buffer);
    }

    JUNE_EXPORT JuneFence juneTextureCreateFence(JuneTexture texture, JuneFenceDescriptor const* descriptor)
    {
        return procTextureCreateFence(texture, descriptor);
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