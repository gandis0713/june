#pragma once

#include "june/june.h"

#include "june/memory/raw_memory.h"

#include <memory>

namespace june
{

class ApiContext;
class Buffer;
class Texture;
class ApiMemory
{
public:
    virtual ~ApiMemory() = default;

    ApiMemory(const ApiMemory&) = delete;
    ApiMemory& operator=(const ApiMemory&) = delete;

public: // June API
    virtual void beginAccess(JuneApiMemoryBeginAccessDescriptor const* descriptor) = 0;
    virtual void endAccess(JuneApiMemoryEndAccessDescriptor const* descriptor) = 0;
    virtual void* createResource(JuneResourceDescriptor const* descriptor) = 0;

protected:
    ApiMemory() = default;
};

} // namespace june