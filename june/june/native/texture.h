#pragma once

#include "june/june.h"

namespace june
{

class TextureMemory;
class Context;
class Texture
{
public:
    virtual ~Texture() = default;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

public: // June API
public:
    virtual Context* getContext() const = 0;
    virtual TextureMemory* getMemory() const = 0;

protected:
    Texture() = default;
};

} // namespace june