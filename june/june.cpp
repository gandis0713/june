#include "june/june.h"

namespace june
{

extern JuneProc procGetProcAddress(StringView name);
extern JuneInstance procCreateInstance(JuneInstanceDescriptor const* desc);

extern JuneSharedMemory procInstanceCreateSharedMemory(JuneInstance instance, JuneSharedMemoryDescriptor const* descriptor);
extern JuneApiContext procInstanceCreateApiContext(JuneInstance instance, JuneApiContextDescriptor const* desc);
extern void procInstanceDestroy(JuneInstance instance);

extern JuneResource procApiContextCreateResource(JuneApiContext context, JuneResourceDescriptor const* descriptor);
extern JuneFence procApiContextCreateFence(JuneApiContext context, JuneFenceDescriptor const* descriptor);
extern void procApiContextDestroy(JuneApiContext context);

extern void procResourceBeginAccess(JuneResource resource, JuneResourceBeginAccessDescriptor const* descriptor);
extern void procResourceEndAccess(JuneResource resource, JuneResourceEndAccessDescriptor const* descriptor);
extern void procResourceConnect(JuneResource srcResource, JuneResource dstResource);
extern void* procResourceGetResource(JuneResource resource, JuneGetResourceDescriptor const* descriptor);
extern void procResourceDestroy(JuneResource resource);

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

    JUNE_EXPORT JuneResource juneApiContextCreateResource(JuneApiContext context, JuneResourceDescriptor const* descriptor)
    {
        return procApiContextCreateResource(context, descriptor);
    }

    JUNE_EXPORT JuneFence juneApiContextCreateFence(JuneApiContext context, JuneFenceDescriptor const* descriptor)
    {
        return procApiContextCreateFence(context, descriptor);
    }

    JUNE_EXPORT void juneApiContextDestroy(JuneApiContext context)
    {
        procApiContextDestroy(context);
    }

    JUNE_EXPORT void juneResourceBeginAccess(JuneResource resource, JuneResourceBeginAccessDescriptor const* descriptor)
    {
        procResourceBeginAccess(resource, descriptor);
    }

    JUNE_EXPORT void juneResourceEndAccess(JuneResource resource, JuneResourceEndAccessDescriptor const* descriptor)
    {
        procResourceEndAccess(resource, descriptor);
    }

    JUNE_EXPORT void juneResourceConnect(JuneResource srcResource, JuneResource dstResource)
    {
        procResourceConnect(srcResource, dstResource);
    }

    JUNE_EXPORT void* juneResourceGetResource(JuneResource resource, JuneGetResourceDescriptor const* descriptor)
    {
        return procResourceGetResource(resource, descriptor);
    }

    JUNE_EXPORT void juneResourceDestroy(JuneResource resource)
    {
        procResourceDestroy(resource);
    }

    JUNE_EXPORT void juneFenceDestroy(JuneFence fence)
    {
        procFenceDestroy(fence);
    }
}