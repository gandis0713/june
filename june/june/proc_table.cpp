#include "june/june.h"

#include "june/native/buffer.h"
#include "june/native/buffer_memory.h"
#include "june/native/context.h"
#include "june/native/fence.h"
#include "june/native/instance.h"
#include "june/native/texture.h"
#include "june/native/texture_memory.h"

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

JuneBufferMemory procApiContextCreateBufferMemory(JuneApiContext context, JuneBufferMemoryDescriptor const* descriptor)
{
    return reinterpret_cast<JuneBufferMemory>(reinterpret_cast<Context*>(context)->createBufferMemory(descriptor));
}

JuneTextureMemory procApiContextCreateTextureMemory(JuneApiContext context, JuneTextureMemoryDescriptor const* descriptor)
{
    return reinterpret_cast<JuneTextureMemory>(reinterpret_cast<Context*>(context)->createTextureMemory(descriptor));
}

void procApiContextDestroy(JuneApiContext context)
{
    if (context)
        delete reinterpret_cast<Context*>(context);
}

JuneBuffer procBufferMemoryCreateBuffer(JuneBufferMemory memory, JuneBufferDescriptor const* descriptor)
{
    return reinterpret_cast<JuneBuffer>(reinterpret_cast<BufferMemory*>(memory)->createBuffer(descriptor));
}

void procBufferMemoryBeginAccess(JuneBufferMemory memory, JuneBeginBufferAccessDescriptor const* descriptor)
{
    reinterpret_cast<BufferMemory*>(memory)->beginAccess(descriptor);
}

void procBufferMemoryEndAccess(JuneBufferMemory memory, JuneEndBufferAccessDescriptor const* descriptor)
{
    reinterpret_cast<BufferMemory*>(memory)->endAccess(descriptor);
}

void procBufferMemoryDestroy(JuneBufferMemory bufferMemory)
{
    if (bufferMemory)
        delete reinterpret_cast<BufferMemory*>(bufferMemory);
}

JuneTexture procTextureMemoryCreateTexture(JuneTextureMemory memory, JuneTextureDescriptor const* descriptor)
{
    return reinterpret_cast<JuneTexture>(reinterpret_cast<TextureMemory*>(memory)->createTexture(descriptor));
}

void procTextureMemoryBeginAccess(JuneTextureMemory memory, JuneBeginTextureAccessDescriptor const* descriptor)
{
    reinterpret_cast<TextureMemory*>(memory)->beginAccess(descriptor);
}

void procTextureMemoryEndAccess(JuneTextureMemory memory, JuneEndTextureAccessDescriptor const* descriptor)
{
    reinterpret_cast<TextureMemory*>(memory)->endAccess(descriptor);
}

void procTextureMemoryDestroy(JuneTextureMemory textureMemory)
{
    if (textureMemory)
        delete reinterpret_cast<TextureMemory*>(textureMemory);
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
    { "juneApiContextCreateBufferMemory", reinterpret_cast<JuneProc>(procApiContextCreateBufferMemory) },
    { "juneApiContextCreateTextureMemory", reinterpret_cast<JuneProc>(procApiContextCreateTextureMemory) },
    { "juneApiContextDestroy", reinterpret_cast<JuneProc>(procApiContextDestroy) },
    { "juneBufferMemoryCreateBuffer", reinterpret_cast<JuneProc>(procBufferMemoryCreateBuffer) },
    { "juneBufferMemoryBeginAccess", reinterpret_cast<JuneProc>(procBufferMemoryBeginAccess) },
    { "juneBufferMemoryEndAccess", reinterpret_cast<JuneProc>(procBufferMemoryEndAccess) },
    { "juneBufferMemoryDestroy", reinterpret_cast<JuneProc>(procBufferMemoryDestroy) },
    { "juneTextureMemoryCreateTexture", reinterpret_cast<JuneProc>(procTextureMemoryCreateTexture) },
    { "juneTextureMemoryBeginAccess", reinterpret_cast<JuneProc>(procTextureMemoryBeginAccess) },
    { "juneTextureMemoryEndAccess", reinterpret_cast<JuneProc>(procTextureMemoryEndAccess) },
    { "juneTextureMemoryDestroy", reinterpret_cast<JuneProc>(procTextureMemoryDestroy) },
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