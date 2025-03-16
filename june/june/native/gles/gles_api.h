#pragma once

namespace june
{

class DyLib;

struct GLESAPI
{
    bool loadInstanceProcs(DyLib* glesLib);
};

} // namespace june