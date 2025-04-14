#include "june/june.h"

#include "june/native/api_context.h"
#include "june/native/fence.h"
#include "june/native/instance.h"
#include "june/native/resource.h"
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

JuneResource procApiContextCreateResource(JuneApiContext context, JuneResourceDescriptor const* descriptor)
{
    return reinterpret_cast<JuneResource>(reinterpret_cast<ApiContext*>(context)->createResource(descriptor));
}

JuneFence procApiContextCreateFence(JuneApiContext context, JuneFenceDescriptor const* descriptor)
{
    // return reinterpret_cast<JuneFence>(reinterpret_cast<ApiContext*>(context)->createFence(descriptor));
    return nullptr;
}

void procApiContextDestroy(JuneApiContext context)
{
    if (context)
        delete reinterpret_cast<ApiContext*>(context);
}

void procResourceBeginAccess(JuneResource resource, JuneResourceBeginAccessDescriptor const* descriptor)
{
    reinterpret_cast<Resource*>(resource)->beginAccess(descriptor);
}

void procResourceEndAccess(JuneResource resource, JuneResourceEndAccessDescriptor const* descriptor)
{
    reinterpret_cast<Resource*>(resource)->endAccess(descriptor);
}

void procResourceConnect(JuneResource srcResource, JuneResource dstResource)
{
    if (srcResource && dstResource)
    {
        reinterpret_cast<Resource*>(srcResource)->connect(reinterpret_cast<Resource*>(dstResource));
    }
}

void* procResourceGetResource(JuneResource resource, JuneGetResourceDescriptor const* descriptor)
{
    return reinterpret_cast<void*>(reinterpret_cast<Resource*>(resource)->getResource(descriptor));
}

void procResourceDestroy(JuneResource resource)
{
    if (resource)
        delete reinterpret_cast<Resource*>(resource);
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
    { "juneApiContextDestroy", reinterpret_cast<JuneProc>(procApiContextDestroy) },
    { "juneResourceBeginAccess", reinterpret_cast<JuneProc>(procResourceBeginAccess) },
    { "juneResourceEndAccess", reinterpret_cast<JuneProc>(procResourceEndAccess) },
    { "juneResourceConnect", reinterpret_cast<JuneProc>(procResourceConnect) },
    { "juneResourceGetResource", reinterpret_cast<JuneProc>(procResourceGetResource) },
    { "juneResourceDestroy", reinterpret_cast<JuneProc>(procResourceDestroy) },
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