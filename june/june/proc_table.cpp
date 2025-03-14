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

namespace
{

std::unordered_map<std::string, JuneProc> sProcMap{
    { "juneCreateInstance", reinterpret_cast<JuneProc>(procCreateInstance) },
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