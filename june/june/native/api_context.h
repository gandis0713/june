#pragma once

#include "june/june.h"

namespace june
{

class SharedMemory;
class Instance;
class Buffer;
class Texture;
class ApiContext
{
public:
    virtual ~ApiContext() = default;

    ApiContext(const ApiContext&) = delete;
    ApiContext& operator=(const ApiContext&) = delete;

public: // June API
    virtual SharedMemory* createSharedMemory(JuneSharedMemoryDescriptor const* descriptor) = 0;
    virtual Buffer* createBuffer(JuneBufferDescriptor const* descriptor) = 0;
    virtual Texture* createTexture(JuneTextureDescriptor const* descriptor) = 0;

public:
    virtual Instance* getInstance() const = 0;
    virtual JuneApiType getApiType() const = 0;

protected:
    ApiContext() = default;
};
} // namespace june