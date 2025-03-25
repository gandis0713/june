#pragma once

#include "june/june.h"

namespace june
{

class ApiMemory;
class Instance;
class Fence;
class ApiContext
{
public:
    virtual ~ApiContext() = default;

    ApiContext(const ApiContext&) = delete;
    ApiContext& operator=(const ApiContext&) = delete;

public: // June API
    virtual ApiMemory* createApiMemory(JuneApiMemoryDescriptor const* descriptor) = 0;
    virtual Fence* createFence(JuneFenceDescriptor const* descriptor) = 0;

public:
    virtual Instance* getInstance() const = 0;
    virtual JuneApiType getApiType() const = 0;

protected:
    ApiContext() = default;
};
} // namespace june