#include <iostream>

#include "base/june_api.h"
#include "june/common/dylib.h"

#include <string>

int main(int argc, char* argv[])
{
    using namespace june;

    DyLib libs{};
    JuneAPI api{};

    std::string libName;
#if defined(__ANDROID__) || defined(ANDROID)
    libName = "libjune.so";
#elif defined(__linux__)
    libName = "libjune.so";
#elif defined(__APPLE__)
    libName = "libjune.dylib";
#elif defined(WIN32)
    libName = "june.dll";
#endif
    if(!libs.open(libName.c_str()))
    {
        throw std::runtime_error("Failed to open library");
    }
    if (!api.loadProcs(&libs))
    {
        throw std::runtime_error("Failed to load procs");
    }


    std::cout << "Hello, june!. Succeed." << std::endl;

    return 0;
}