#include "june/june.h"

namespace june
{

extern JuneProc procGetProcAddress(StringView name);
extern JuneInstance procCreateInstance(JuneInstanceDescriptor const* desc);

extern JuneSharedMemory procInstanceCreateSharedMemory(JuneInstance instance, JuneSharedMemoryDescriptor const* descriptor);
extern JuneApiContext procInstanceCreateApiContext(JuneInstance instance, JuneApiContextDescriptor const* desc);
extern void procInstanceDestroy(JuneInstance instance);

extern JuneApiMemory procApiContextCreateApiMemory(JuneApiContext context, JuneApiMemoryDescriptor const* descriptor);
extern JuneFence procApiContextCreateFence(JuneApiContext context, JuneFenceDescriptor const* descriptor);
extern void procApiContextDestroy(JuneApiContext context);

extern void procApiMemoryBeginAccess(JuneApiMemory memory, JuneApiMemoryBeginAccessDescriptor const* descriptor);
extern void procApiMemoryEndAccess(JuneApiMemory memory, JuneApiMemoryEndAccessDescriptor const* descriptor);
extern void procApiMemoryConnect(JuneApiMemory srcMemory, JuneApiMemory dstMemory);
extern void* procApiMemoryCreateResource(JuneApiMemory memory, JuneResourceDescriptor const* descriptor);
extern void procApiMemoryDestroy(JuneApiMemory memory);

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

    JUNE_EXPORT JuneSharedMemory juneInstanceCreateSharedMemory(JuneInstance instance, JuneSharedMemoryDescriptor const* descriptor)
    {
        return procInstanceCreateSharedMemory(instance, descriptor);
    }

    JUNE_EXPORT JuneApiContext juneInstanceCreateApiContext(JuneInstance instance, JuneApiContextDescriptor const* desc)
    {
        return procInstanceCreateApiContext(instance, desc);
    }

    JUNE_EXPORT void juneInstanceDestroy(JuneInstance instance)
    {
        procInstanceDestroy(instance);
    }

    JUNE_EXPORT JuneApiMemory juneApiContextCreateApiMemory(JuneApiContext context, JuneApiMemoryDescriptor const* descriptor)
    {
        return procApiContextCreateApiMemory(context, descriptor);
    }

    JUNE_EXPORT JuneFence juneApiContextCreateFence(JuneApiContext context, JuneFenceDescriptor const* descriptor)
    {
        return procApiContextCreateFence(context, descriptor);
    }

    JUNE_EXPORT void juneApiContextDestroy(JuneApiContext context)
    {
        procApiContextDestroy(context);
    }

    JUNE_EXPORT void juneApiMemoryBeginAccess(JuneApiMemory memory, JuneApiMemoryBeginAccessDescriptor const* descriptor)
    {
        procApiMemoryBeginAccess(memory, descriptor);
    }

    JUNE_EXPORT void juneApiMemoryEndAccess(JuneApiMemory memory, JuneApiMemoryEndAccessDescriptor const* descriptor)
    {
        procApiMemoryEndAccess(memory, descriptor);
    }

    JUNE_EXPORT void juneApiMemoryConnect(JuneApiMemory srcMemory, JuneApiMemory dstMemory)
    {
        procApiMemoryConnect(srcMemory, dstMemory);
    }

    JUNE_EXPORT void* juneApiMemoryCreateResource(JuneApiMemory memory, JuneResourceDescriptor const* descriptor)
    {
        return procApiMemoryCreateResource(memory, descriptor);
    }

    JUNE_EXPORT void juneApiMemoryDestroy(JuneApiMemory memory)
    {
        procApiMemoryDestroy(memory);
    }

    JUNE_EXPORT void juneFenceDestroy(JuneFence fence)
    {
        procFenceDestroy(fence);
    }
}