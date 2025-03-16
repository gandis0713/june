#include "june/june.h"

#include "june/native/buffer.h"
#include "june/native/buffer_memory.h"
#include "june/native/context.h"
#include "june/native/instance.h"
#include "june/native/texture.h"
#include "june/native/texture_memory.h"

#include <string>
#include <unordered_map>

namespace june
{

JuneInstance procCreateInstance(JuneInstanceDescriptor const* descriptor)
{
    return reinterpret_cast<JuneInstance>(Instance::create(descriptor));
}

void procDestroyInstance(JuneInstance instance)
{
    if (instance)
        delete reinterpret_cast<Instance*>(instance);
}

JuneApiContext procCreateApiContext(JuneInstance innstance, JuneApiContextDescriptor const* desc)
{
    return reinterpret_cast<JuneApiContext>(reinterpret_cast<Instance*>(innstance)->createApiContext(desc));
}

void procDestroyApiContext(JuneApiContext context)
{
    if (context)
        delete reinterpret_cast<JuneApiContext*>(context);
}

JuneBufferMemory procCreateBufferMemory(JuneApiContext context, JuneBufferMemoryDescriptor const* descriptor)
{
    return reinterpret_cast<JuneBufferMemory>(reinterpret_cast<Context*>(context)->createBufferMemory(descriptor));
}

JuneTextureMemory procCreateTextureMemory(JuneApiContext context, JuneTextureMemoryDescriptor const* descriptor)
{
    return reinterpret_cast<JuneTextureMemory>(reinterpret_cast<Context*>(context)->createTextureMemory(descriptor));
}

void procDestroyBufferMemory(JuneBufferMemory bufferMemory)
{
    if (bufferMemory)
        delete reinterpret_cast<BufferMemory*>(bufferMemory);
}

void procDestroyTextureMemory(JuneTextureMemory textureMemory)
{
    if (textureMemory)
        delete reinterpret_cast<TextureMemory*>(textureMemory);
}

JuneBuffer procCreateBuffer(JuneBufferMemory memory, JuneBufferDescriptor const* descriptor)
{
    return reinterpret_cast<JuneBuffer>(reinterpret_cast<BufferMemory*>(memory)->createBuffer(descriptor));
}

void procDestroyBuffer(JuneBuffer buffer)
{
    if (buffer)
        delete reinterpret_cast<Buffer*>(buffer);
}

JuneTexture procCreateTexture(JuneTextureMemory memory, JuneTextureDescriptor const* descriptor)
{
    return reinterpret_cast<JuneTexture>(reinterpret_cast<TextureMemory*>(memory)->createTexture(descriptor));
}

void procDestroyTexture(JuneTexture texture)
{
    if (texture)
        delete reinterpret_cast<Texture*>(texture);
}

namespace
{

std::unordered_map<std::string, JuneProc> sProcMap{
    { "juneCreateInstance", reinterpret_cast<JuneProc>(procCreateInstance) },
    { "juneDestroyInstance", reinterpret_cast<JuneProc>(procDestroyInstance) },
    { "juneCreateApiContext", reinterpret_cast<JuneProc>(procCreateApiContext) },
    { "juneDestroyApiContext", reinterpret_cast<JuneProc>(procDestroyApiContext) },
    { "juneCreateBufferMemory", reinterpret_cast<JuneProc>(procCreateBufferMemory) },
    { "juneDestroyBufferMemory", reinterpret_cast<JuneProc>(procDestroyBufferMemory) },
    { "juneCreateTextureMemory", reinterpret_cast<JuneProc>(procCreateTextureMemory) },
    { "juneDestroyTextureMemory", reinterpret_cast<JuneProc>(procDestroyTextureMemory) },
    { "juneCreateBuffer", reinterpret_cast<JuneProc>(procCreateBuffer) },
    { "juneDestroyBuffer", reinterpret_cast<JuneProc>(procDestroyBuffer) },
    { "juneCreateTexture", reinterpret_cast<JuneProc>(procCreateTexture) },
    { "juneDestroyTexture", reinterpret_cast<JuneProc>(procDestroyTexture) },
};

} // namespace

JuneProc procGetProcAddress(char const* procName)
{
    if (procName == nullptr)
    {
        return nullptr;
    }

    std::string key(procName);
    if (sProcMap.contains(key))
    {
        return sProcMap[key];
    }

    return nullptr;
}

} // namespace june