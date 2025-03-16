#include "gles_api.h"

#include "june/common/dylib.h"
#include <spdlog/spdlog.h>

namespace june
{
bool GLESAPI::loadInstanceProcs(DyLib* glesLib)
{
    // #define GET_GLOBAL_PROC(name)                                                          \
//     name = reinterpret_cast<decltype(name)>(GetInstanceProcAddr(nullptr, "vk" #name)); \
//     if (name == nullptr)                                                               \
//     {                                                                                  \
//         spdlog::error("Couldn't get instance proc vk{}", #name);                       \
//         return false;                                                                  \
//     }

    //     if (!glesLib->getProc(&GetInstanceProcAddr, "vkGetInstanceProcAddr"))
    //     {
    //         spdlog::error("Couldn't get vkGetInstanceProcAddr");
    //         return false;
    //     }

    return true;
}

} // namespace june