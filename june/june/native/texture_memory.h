#pragma once

#include "june/june.h"

namespace june
{

class Context;
class Texture;
class TextureMemory
{
public:
    virtual ~TextureMemory() = default;

    TextureMemory(const TextureMemory&) = delete;
    TextureMemory& operator=(const TextureMemory&) = delete;

public: // June API
    virtual Texture* createTexture(JuneTextureDescriptor const* descriptor) = 0;

public:
    virtual Context* getContext() const = 0;

protected:
    TextureMemory() = default;
};

} // namespace june