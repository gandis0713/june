#pragma once

#include "june/june.h"

namespace june
{

class Context;
class Buffer;
class Texture;
class SharedMemory
{
public:
    virtual ~SharedMemory() = default;

    SharedMemory(const SharedMemory&) = delete;
    SharedMemory& operator=(const SharedMemory&) = delete;

public: // June API
    virtual Buffer* createBuffer(JuneBufferDescriptor const* descriptor) = 0;
    virtual Texture* createTexture(JuneTextureDescriptor const* descriptor) = 0;
    virtual void beginAccess(JuneBeginAccessDescriptor const* descriptor) = 0;
    virtual void endAccess(JuneEndAccessDescriptor const* descriptor) = 0;

public:
    virtual Context* getContext() const = 0;

protected:
    SharedMemory() = default;
};

} // namespace june