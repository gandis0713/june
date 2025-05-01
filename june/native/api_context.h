#pragma once

#include "june/june.h"
#include "object.h"

namespace june
{

class Resource;
class Instance;
class Fence;
class ApiContext : public Object
{
public:
    ApiContext() = delete;
    virtual ~ApiContext() = default;

    ApiContext(const ApiContext&) = delete;
    ApiContext& operator=(const ApiContext&) = delete;

public: // June API
    virtual void createResource(JuneResourceDescriptor const* descriptor) = 0;
    virtual Fence* createFence(JuneFenceDescriptor const* descriptor) = 0;
    virtual void beginMemoryAccess(JuneApiContextBeginMemoryAccessDescriptor const* descriptor) = 0;
    virtual void endMemoryAccess(JuneApiContextEndMemoryAccessDescriptor const* descriptor) = 0;

public:
    virtual JuneApiType getApiType() const = 0;

public:
    Instance* getInstance() const;

protected:
    ApiContext(Instance* instance, JuneApiContextDescriptor const* descriptor);

protected:
    Instance* m_instance{ nullptr };
    const JuneApiContextDescriptor m_descriptor;
};

} // namespace june