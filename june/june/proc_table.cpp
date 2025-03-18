#include "june/june.h"

#include "june/native/api_context.h"
#include "june/native/buffer.h"
#include "june/native/fence.h"
#include "june/native/instance.h"
#include "june/native/shared_memory.h"
#include "june/native/texture.h"

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

void procInstanceDestroy(JuneInstance instance)
{
    if (instance)
        delete reinterpret_cast<Instance*>(instance);
}

JuneSharedMemory procApiContextCreateSharedMemory(JuneApiContext context, JuneSharedMemoryDescriptor const* descriptor)
{
    return reinterpret_cast<JuneSharedMemory>(reinterpret_cast<ApiContext*>(context)->createSharedMemory(descriptor));
}

JuneBuffer procApiContextCreateBuffer(JuneApiContext context, JuneBufferDescriptor const* descriptor)
{
    return reinterpret_cast<JuneBuffer>(reinterpret_cast<ApiContext*>(context)->createBuffer(descriptor));
}

JuneTexture procApiContextCreateTexture(JuneApiContext context, JuneTextureDescriptor const* descriptor)
{
    return reinterpret_cast<JuneTexture>(reinterpret_cast<ApiContext*>(context)->createTexture(descriptor));
}

void procApiContextDestroy(JuneApiContext context)
{
    if (context)
        delete reinterpret_cast<ApiContext*>(context);
}

void procSharedMemoryBeginAccess(JuneSharedMemory memory, JuneBeginAccessDescriptor const* descriptor)
{
    reinterpret_cast<SharedMemory*>(memory)->beginAccess(descriptor);
}

void procSharedMemoryEndAccess(JuneSharedMemory memory, JuneEndAccessDescriptor const* descriptor)
{
    reinterpret_cast<SharedMemory*>(memory)->endAccess(descriptor);
}

void procSharedMemoryDestroy(JuneSharedMemory bufferMemory)
{
    if (bufferMemory)
        delete reinterpret_cast<SharedMemory*>(bufferMemory);
}

JuneFence procBufferCreateFence(JuneBuffer buffer, JuneFenceDescriptor const* descriptor)
{
    return reinterpret_cast<JuneFence>(reinterpret_cast<Buffer*>(buffer)->createFence(descriptor));
}

void procBufferDestroy(JuneBuffer buffer)
{
    if (buffer)
        delete reinterpret_cast<Buffer*>(buffer);
}

JuneFence procTextureCreateFence(JuneTexture texture, JuneFenceDescriptor const* descriptor)
{
    return reinterpret_cast<JuneFence>(reinterpret_cast<Texture*>(texture)->createFence(descriptor));
}

void procTextureDestroy(JuneTexture texture)
{
    if (texture)
        delete reinterpret_cast<Texture*>(texture);
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
    { "juneInstanceDestroy", reinterpret_cast<JuneProc>(procInstanceDestroy) },
    { "juneApiContextCreateSharedMemory", reinterpret_cast<JuneProc>(procApiContextCreateSharedMemory) },
    { "juneApiContextCreateBuffer", reinterpret_cast<JuneProc>(procApiContextCreateBuffer) },
    { "juneApiContextCreateTexture", reinterpret_cast<JuneProc>(procApiContextCreateTexture) },
    { "juneApiContextDestroy", reinterpret_cast<JuneProc>(procApiContextDestroy) },
    { "juneSharedMemoryBeginAccess", reinterpret_cast<JuneProc>(procSharedMemoryBeginAccess) },
    { "juneSharedMemoryEndAccess", reinterpret_cast<JuneProc>(procSharedMemoryEndAccess) },
    { "juneSharedMemoryDestroy", reinterpret_cast<JuneProc>(procSharedMemoryDestroy) },
    { "juneBufferCreateFence", reinterpret_cast<JuneProc>(procBufferCreateFence) },
    { "juneBufferDestroy", reinterpret_cast<JuneProc>(procBufferDestroy) },
    { "juneTextureCreateFence", reinterpret_cast<JuneProc>(procTextureCreateFence) },
    { "juneTextureDestroy", reinterpret_cast<JuneProc>(procTextureDestroy) },
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