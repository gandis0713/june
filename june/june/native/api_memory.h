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
    ApiMemory() = delete;
    virtual ~ApiMemory() = default;

    ApiMemory(const ApiMemory&) = delete;
    ApiMemory& operator=(const ApiMemory&) = delete;

public: // June API
    virtual void beginAccess(JuneBeginAccessDescriptor const* descriptor) = 0;
    virtual void endAccess(JuneEndAccessDescriptor const* descriptor) = 0;

public:
    ApiContext* getContext() const;

protected:
    ApiMemory(ApiContext* context, JuneApiMemoryDescriptor const* descriptor);

protected:
    ApiContext* m_context{ nullptr };
    const JuneApiMemoryDescriptor m_descriptor;
};

} // namespace june