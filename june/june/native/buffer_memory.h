#pragma once

namespace june
{

class Context;
class Buffer;
class BufferMemory
{
public:
    virtual ~BufferMemory() = default;

    BufferMemory(const BufferMemory&) = delete;
    BufferMemory& operator=(const BufferMemory&) = delete;

public: // June API
    virtual Buffer* createBuffer(JuneBufferDescriptor const* descriptor) = 0;
    virtual void beginAccess(JuneBeginBufferAccessDescriptor const* descriptor) = 0;
    virtual void endAccess(JuneEndBufferAccessDescriptor const* descriptor) = 0;

public:
    virtual Context* getContext() const = 0;

protected:
    BufferMemory() = default;
};

} // namespace june