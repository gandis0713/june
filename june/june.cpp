#include "june/june.h"

namespace june
{

extern JuneProc procGetProcAddress(StringView name);
extern JuneInstance procCreateInstance(JuneInstanceDescriptor const* desc);

extern JuneSharedMemory procInstanceImportSharedMemory(JuneInstance instance, JuneSharedMemoryImportDescriptor const* descriptor);
extern JuneFence procInstanceCreateFence(JuneInstance instance, JuneFenceCreateDescriptor const* descriptor);
extern JuneApiContext procInstanceCreateApiContext(JuneInstance instance, JuneApiContextDescriptor const* desc);
extern void procInstanceDestroy(JuneInstance instance);

extern void procApiContextCreateResource(JuneApiContext apiContext, JuneResourceCreateDescriptor const* descriptor);
extern void procApiContextExportFence(JuneApiContext apiContext, JuneFenceExportDescriptor const* descriptor);
extern void procApiContextDestroy(JuneApiContext apiContext);

extern void procSharedMemoryDestroy(JuneSharedMemory sharedMemory);

extern void procFenceReset(JuneFence fence, JuneFenceResetDescriptor const* descriptor);
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

    JUNE_EXPORT JuneSharedMemory juneInstanceImportSharedMemory(JuneInstance instance, JuneSharedMemoryImportDescriptor const* descriptor)
    {
        return procInstanceImportSharedMemory(instance, descriptor);
    }


    JUNE_EXPORT JuneApiContext juneInstanceCreateApiContext(JuneInstance instance, JuneApiContextDescriptor const* desc)
    {
        return procInstanceCreateApiContext(instance, desc);
    }

    JUNE_EXPORT void juneInstanceDestroy(JuneInstance instance)
    {
        procInstanceDestroy(instance);
    }

    JUNE_EXPORT void juneApiContextCreateResource(JuneApiContext apiContext, JuneResourceCreateDescriptor const* descriptor)
    {
        return procApiContextCreateResource(apiContext, descriptor);
    }


    JUNE_EXPORT JuneFence juneInstanceCreateFence(JuneInstance instance, JuneFenceCreateDescriptor const* descriptor)
    {
        return procInstanceCreateFence(instance, descriptor);
    }

    JUNE_EXPORT void juneApiContextExportFence(JuneApiContext apiContext, JuneFenceExportDescriptor const* descriptor)
    {
        procApiContextExportFence(apiContext, descriptor);
    }

    JUNE_EXPORT void juneApiContextDestroy(JuneApiContext apiContext)
    {
        procApiContextDestroy(apiContext);
    }

    JUNE_EXPORT void juneSharedMemoryDestroy(JuneSharedMemory sharedMemory)
    {
        procSharedMemoryDestroy(sharedMemory);
    }

    JUNE_EXPORT void juneFenceReset(JuneFence fence, JuneFenceResetDescriptor const* descriptor)
    {
        procFenceReset(fence, descriptor);
    }

    JUNE_EXPORT void juneFenceDestroy(JuneFence fence)
    {
        procFenceDestroy(fence);
    }
}