#include "june/june.h"

#include <unordered_map>
#include <string>

namespace june
{

JuneInstance procCreateInstance(JuneInstanceDescriptor const* wgpuDescriptor)
{
    return nullptr;
}

namespace
{

std::unordered_map<std::string, JuneProc> sProcMap{
    { "wgpuCreateInstance", reinterpret_cast<JuneProc>(procCreateInstance) },
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