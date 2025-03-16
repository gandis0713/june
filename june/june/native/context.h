#pragma once

#include "june/june.h"

namespace june
{

class TextureMemory;
class BufferMemory;
class Instance;
class Context
{
public:
    virtual ~Context() = default;

    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

public: // June API
    virtual BufferMemory* createBufferMemory(JuneBufferMemoryDescriptor const* descriptor) = 0;
    virtual TextureMemory* createTextureMemory(JuneTextureMemoryDescriptor const* descriptor) = 0;

public:
    virtual Instance* getInstance() const = 0;
    virtual JuneApiType getApiType() const = 0;

protected:
    Context() = default;
};
} // namespace june