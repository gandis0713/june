#include "june/june.h"

#include "june/native/api_context.h"
#include "june/native/fence.h"
#include "june/native/instance.h"
#include "june/native/shared_memory.h"

#include <string>
#include <unordered_map>

namespace june
{

JuneInstance procCreateInstance(JuneInstanceDescriptor const* desc)
{
    return reinterpret_cast<JuneInstance>(Instance::create(desc));
}

JuneApiContext procInstanceCreateApiContext(JuneInstance instance, JuneApiContextDescriptor const* desc)
{
    return reinterpret_cast<JuneApiContext>(reinterpret_cast<Instance*>(instance)->createApiContext(desc));
}

JuneSharedMemory procInstanceCreateSharedMemory(JuneInstance instance, JuneSharedMemoryDescriptor const* descriptor)
{
    return reinterpret_cast<JuneSharedMemory>(reinterpret_cast<Instance*>(instance)->createSharedMemory(descriptor));
}

void procInstanceDestroy(JuneInstance instance)
{
    if (instance)
        delete reinterpret_cast<Instance*>(instance);
}

void procApiContextCreateResource(JuneApiContext apiContext, JuneResourceDescriptor const* descriptor)
{
    return reinterpret_cast<ApiContext*>(apiContext)->createResource(descriptor);
}

JuneFence procApiContextCreateFence(JuneApiContext apiContext, JuneFenceDescriptor const* descriptor)
{
    return reinterpret_cast<JuneFence>(reinterpret_cast<ApiContext*>(apiContext)->createFence(descriptor));
}

void procApiContextBeginMemoryAccess(JuneApiContext apiContext, JuneApiContextBeginMemoryAccessDescriptor const* descriptor)
{
    reinterpret_cast<ApiContext*>(apiContext)->beginMemoryAccess(descriptor);
}

void procApiContextEndMemoryAccess(JuneApiContext apiContext, JuneApiContextEndMemoryAccessDescriptor const* descriptor)
{
    reinterpret_cast<ApiContext*>(apiContext)->endMemoryAccess(descriptor);
}

void procApiContextDestroy(JuneApiContext apiContext)
{
    if (apiContext)
        delete reinterpret_cast<ApiContext*>(apiContext);
}

void procSharedMemoryDestroy(JuneSharedMemory sharedMemory)
{
    if (sharedMemory)
        delete reinterpret_cast<SharedMemory*>(sharedMemory);
}

void procFenceDestroy(JuneFence fence)
{
    if (fence)
        delete reinterpret_cast<Fence*>(fence);
}

namespace
{

std::unordered_map<std::string, JuneProc> sProcMap{
    { "juneCreateInstance", reinterpret_cast<JuneProc>(procCreateInstance) },
    { "juneInstanceCreateSharedMemory", reinterpret_cast<JuneProc>(procInstanceCreateSharedMemory) },
    { "juneInstanceCreateApiContext", reinterpret_cast<JuneProc>(procInstanceCreateApiContext) },
    { "juneInstanceDestroy", reinterpret_cast<JuneProc>(procInstanceDestroy) },
    { "juneApiContextCreateResource", reinterpret_cast<JuneProc>(procApiContextCreateResource) },
    { "juneApiContextCreateFence", reinterpret_cast<JuneProc>(procApiContextCreateFence) },
    { "juneApiContextBeginMemoryAccess", reinterpret_cast<JuneProc>(procApiContextBeginMemoryAccess) },
    { "juneApiContextEndMemoryAccess", reinterpret_cast<JuneProc>(procApiContextEndMemoryAccess) },
    { "juneApiContextDestroy", reinterpret_cast<JuneProc>(procApiContextDestroy) },
    { "juneSharedMemoryDestroy", reinterpret_cast<JuneProc>(procSharedMemoryDestroy) },
    { "juneFenceDestroy", reinterpret_cast<JuneProc>(procFenceDestroy) },
};

} // namespace

JuneProc procGetProcAddress(StringView procName)
{
    if (procName.data == nullptr)
    {
        return nullptr;
    }

    std::string key(procName.data);
    if (sProcMap.contains(key))
    {
        return sProcMap[key];
    }

    return nullptr;
}

} // namespace june