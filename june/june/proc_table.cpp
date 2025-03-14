#include "june/june.h"

#include "june/native/instance.h"

#include <unordered_map>
#include <string>

namespace june
{

JuneInstance procCreateInstance(JuneInstanceDescriptor const* descriptor)
{
    return reinterpret_cast<JuneInstance>(Instance::create(descriptor));
}

JuneApiContext procCreateApiContext(JuneInstance innstance, JuneApiContextDescriptor const* desc)
{
    return nullptr;
    // return reinterpret_cast<JuneApiContext>(reinterpret_cast<Instance*>(innstance)->createApiContext(desc));
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