#include <iostream>

#include "base/june_api.h"
#include "june/common/dylib.h"
#include "june/common/vulkan_api.h"

#include <string>

int main(int argc, char* argv[])
{
    using namespace june;

    DyLib juneLibs{};
    JuneAPI juneApi{};

    std::string juneLibName;
#if defined(__ANDROID__) || defined(ANDROID)
    juneLibName = "libjune.so";
#elif defined(__linux__)
    juneLibName = "libjune.so";
#elif defined(__APPLE__)
    juneLibName = "libjune.dylib";
#elif defined(WIN32)
    juneLibName = "june.dll";
#endif
    if (!juneLibs.open(juneLibName.c_str()))
    {
        throw std::runtime_error("Failed to open library");
    }
    if (!juneApi.loadProcs(&juneLibs))
    {
        throw std::runtime_error("Failed to load procs");
    }

    DyLib vulkanLibs{};
    VulkanAPI vulkanApi{};

    std::string vulkanLibName;
#if defined(__ANDROID__) || defined(ANDROID)
    vulkanLibName = "libvulkan.so";
#elif defined(__linux__)
    vulkanLibName = "libvulkan.so.1";
#elif defined(__APPLE__)
    vulkanLibName = "libvulkan.dylib";
#elif defined(WIN32)
    vulkanLibName = "vulkan-1.dll";
#endif

    if (!vulkanLibs.open(vulkanLibName.c_str()))
    {
        throw std::runtime_error("Failed to open library");
    }
    if (!vulkanApi.loadInstanceProcs(&vulkanLibs))
    {
        throw std::runtime_error("Failed to load procs");
    }

    std::cout << "Hello, june!. Succeed." << std::endl;

    return 0;
}