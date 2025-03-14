#pragma once

#include "june/june.h"

namespace june
{

class DyLib;
struct JuneAPI
{
    bool loadProcs(DyLib* JuneLib);

    JuneProcCreateInstance CreateInstance = nullptr;
    JuneProcGetProcAddress GetProcAddress = nullptr;
};

} // namespace june