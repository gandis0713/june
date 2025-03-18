#pragma once

#include "june/june.h"

namespace june
{

class SharedMemory;
class ApiContext;
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
    virtual ApiContext* getApiContext() const = 0;

protected:
    Texture() = default;
};

} // namespace june