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
    // return reinterpret_cast<JuneSharedMemory>(reinterpret_cast<Instance*>(instance)->createSharedMemory(descriptor));
    return nullptr;
}

void procInstanceDestroy(JuneInstance instance)
{
    if (instance)
        delete reinterpret_cast<Instance*>(instance);
}

JuneApiMemory procApiContextCreateApiMemory(JuneApiContext context, JuneApiMemoryDescriptor const* descriptor)
{
    return reinterpret_cast<JuneApiMemory>(reinterpret_cast<ApiContext*>(context)->createApiMemory(descriptor));
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

void procApiMemoryBeginAccess(JuneApiMemory memory, JuneBeginAccessDescriptor const* descriptor)
{
    // reinterpret_cast<ApiMemory*>(memory)->beginAccess(descriptor);
}

void procApiMemoryEndAccess(JuneApiMemory memory, JuneEndAccessDescriptor const* descriptor)
{
    // reinterpret_cast<ApiMemory*>(memory)->endAccess(descriptor);
}

void* procApiMemoryCreateBuffer(JuneApiMemory memory, JuneBufferDescriptor const* descriptor)
{
    // return reinterpret_cast<void*>(reinterpret_cast<ApiMemory*>(memory)->createBuffer(descriptor));
    return nullptr;
}

void* procApiMemoryCreateTexture(JuneApiMemory memory, JuneTextureDescriptor const* descriptor)
{
    // return reinterpret_cast<void*>(reinterpret_cast<ApiMemory*>(memory)->createTexture(descriptor));
    return nullptr;
}

void procApiMemoryDestroy(JuneApiMemory memory)
{
    // if (memory)
    // delete reinterpret_cast<ApiMemory*>(memory);
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
    { "juneInstanceCreateApiContext", reinterpret_cast<JuneProc>(procInstanceCreateApiContext) },
    { "juneInstanceCreateSharedMemory", reinterpret_cast<JuneProc>(procInstanceCreateSharedMemory) },
    { "juneInstanceDestroy", reinterpret_cast<JuneProc>(procInstanceDestroy) },
    { "juneApiContextCreateApiMemory", reinterpret_cast<JuneProc>(procApiContextCreateApiMemory) },
    { "juneApiContextCreateFence", reinterpret_cast<JuneProc>(procApiContextCreateFence) },
    { "juneApiContextDestroy", reinterpret_cast<JuneProc>(procApiContextDestroy) },
    { "juneApiMemoryBeginAccess", reinterpret_cast<JuneProc>(procApiMemoryBeginAccess) },
    { "juneApiMemoryEndAccess", reinterpret_cast<JuneProc>(procApiMemoryEndAccess) },
    { "juneApiMemoryCreateBuffer", reinterpret_cast<JuneProc>(procApiMemoryCreateBuffer) },
    { "juneApiMemoryCreateTexture", reinterpret_cast<JuneProc>(procApiMemoryCreateTexture) },
    { "juneApiMemoryDestroy", reinterpret_cast<JuneProc>(procApiMemoryDestroy) },
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