#pragma once

#include "june/june.h"

namespace june
{

class BufferMemory;
class Context;
class Buffer
{
public:
    virtual ~Buffer() = default;

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

public: // June API
public:
    virtual Context* getContext() const = 0;
    virtual BufferMemory* getMemory() const = 0;

protected:
    Buffer() = default;
};

} // namespace june