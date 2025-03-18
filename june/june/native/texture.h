#pragma once

#include "june/june.h"

namespace june
{

class SharedMemory;
class Context;
class Fence;
class Texture
{
public:
    virtual ~Texture() = default;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

public: // June API
    virtual Fence* createFence(JuneFenceDescriptor const* descriptor) = 0;

public:
    virtual Context* getContext() const = 0;
    virtual SharedMemory* getMemory() const = 0;

protected:
    Texture() = default;
};

} // namespace june