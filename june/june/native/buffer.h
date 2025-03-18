#pragma once

#include "june/june.h"

namespace june
{

class SharedMemory;
class ApiContext;
class Fence;
class Buffer
{
public:
    virtual ~Buffer() = default;

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

public: // June API
    virtual Fence* createFence(JuneFenceDescriptor const* descriptor) = 0;
    virtual void* getVkBuffer() const = 0;

public:
    virtual ApiContext* getApiContext() const = 0;

protected:
    Buffer() = default;
};

} // namespace june