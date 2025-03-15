#include "june/june.h"

#include "june/native/instance.h"

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
    return nullptr;
    // return reinterpret_cast<JuneApiContext>(reinterpret_cast<Instance*>(innstance)->createApiContext(desc));
}

void procDestroyApiContext(JuneApiContext context)
{
    // reinterpret_cast<ApiContext*>(context)->destroy();
}

JuneBuffer procCreateBuffer(JuneApiContext context, JuneBufferDescriptor const* descriptor)
{
    return nullptr;
    // return reinterpret_cast<JuneBuffer>(reinterpret_cast<ApiContext*>(context)->createBuffer(descriptor));
}

void procDestroyBuffer(JuneBuffer buffer)
{
    // reinterpret_cast<Buffer*>(buffer)->destroy();
}

JuneTexture procCreateTexture(JuneApiContext context, JuneTextureDescriptor const* descriptor)
{
    return nullptr;
    // return reinterpret_cast<JuneTexture>(reinterpret_cast<ApiContext*>(context)->createTexture(descriptor));
}

void procDestroyTexture(JuneTexture texture)
{
    // reinterpret_cast<Texture*>(texture)->destroy();
}

namespace
{

std::unordered_map<std::string, JuneProc> sProcMap{
    { "juneCreateInstance", reinterpret_cast<JuneProc>(procCreateInstance) },
    { "juneCreateApiContext", reinterpret_cast<JuneProc>(procCreateApiContext) },
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