#pragma once

namespace june
{

class Context;
class Buffer
{
public:
    virtual ~Buffer() = default;

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

public:
    // June API

public:
    virtual Context* getContext() const = 0;

protected:
    Buffer() = default;
};

} // namespace june