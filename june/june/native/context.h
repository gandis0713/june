#pragma once

#include "june/june.h"

namespace june
{

class Texture;
class Buffer;
class Instance;
class Context
{
public:
    virtual ~Context() = default;

    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

public:
    virtual Buffer* createBuffer(JuneBufferDescriptor const* descriptor) = 0;
    virtual Texture* createTexture(JuneTextureDescriptor const* descriptor) = 0;

public:
    virtual Instance* getInstance() const = 0;

protected:
    Context() = default;
};
} // namespace june