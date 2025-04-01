#pragma once

#include "june/june.h"
#include "june/native/api_memory.h"

namespace june
{

class GLESApiContext;
class GLESApiMemory : public ApiMemory
{
public:
    GLESApiMemory() = delete;
    ~GLESApiMemory() override = default;

    GLESApiMemory(const GLESApiMemory&) = delete;
    GLESApiMemory& operator=(const GLESApiMemory&) = delete;

protected:
    GLESApiMemory(GLESApiContext* context, JuneApiMemoryDescriptor const* descriptor);
};

} // namespace june