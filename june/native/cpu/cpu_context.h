#pragma once

#include "june/common/cast.h"
#include "june/common/dylib.h"
#include "june/native/api_context.h"

#include "june/june.h"

#include <memory>
#include <vector>

namespace june
{

class Instance;
class CPUContext : public ApiContext
{
public:
    static ApiContext* create(Instance* instance, JuneApiContextDescriptor const* descriptor);

public:
    CPUContext() = delete;
    ~CPUContext() override;

    CPUContext(const CPUContext&) = delete;
    CPUContext& operator=(const CPUContext&) = delete;

public: // June API
    void createResource(JuneResourceDescriptor const* descriptor) override;
    Fence* createFence(JuneFenceDescriptor const* descriptor) override;
    void beginMemoryAccess(JuneApiContextBeginMemoryAccessDescriptor const* descriptor) override;
    void endMemoryAccess(JuneApiContextEndMemoryAccessDescriptor const* descriptor) override;

public:
    JuneApiType getApiType() const override;

private:
    CPUContext(Instance* instance, JuneApiContextDescriptor const* descriptor);
};

} // namespace june
