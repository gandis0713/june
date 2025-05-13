#pragma once

#include "june/june.h"
#include "object.h"

namespace june
{

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
    virtual void createResource(JuneResourceCreateDescriptor const* descriptor) = 0;
    virtual Fence* createFence(JuneFenceCreateDescriptor const* descriptor) = 0;
    virtual void exportFence(JuneFenceExportDescriptor const* descriptor) = 0;

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