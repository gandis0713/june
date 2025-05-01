#pragma once

#include "june/june.h"

namespace june
{

class Resource;
class Instance;
class Fence;
class ApiContext
{
public:
    virtual ~ApiContext() = default;

    ApiContext(const ApiContext&) = delete;
    ApiContext& operator=(const ApiContext&) = delete;

public: // June API
    virtual void createResource(JuneResourceDescriptor const* descriptor) = 0;
    virtual Fence* createFence(JuneFenceDescriptor const* descriptor) = 0;
    virtual void beginMemoryAccess(JuneApiContextBeginMemoryAccessDescriptor const* descriptor) = 0;
    virtual void endMemoryAccess(JuneApiContextEndMemoryAccessDescriptor const* descriptor) = 0;

public:
    virtual Instance* getInstance() const = 0;
    virtual JuneApiType getApiType() const = 0;

protected:
    ApiContext() = default;
};
} // namespace june