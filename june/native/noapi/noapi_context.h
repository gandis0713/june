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
class NoApiContext : public ApiContext
{
public:
    static ApiContext* create(Instance* instance, JuneApiContextDescriptor const* descriptor);

public:
    NoApiContext() = delete;
    ~NoApiContext() override;

    NoApiContext(const NoApiContext&) = delete;
    NoApiContext& operator=(const NoApiContext&) = delete;

public: // June API
    void createResource(JuneResourceDescriptor const* descriptor) override;
    Fence* createFence(JuneFenceDescriptor const* descriptor) override;
    void beginMemoryAccess(JuneApiContextBeginMemoryAccessDescriptor const* descriptor) override;
    void endMemoryAccess(JuneApiContextEndMemoryAccessDescriptor const* descriptor) override;

public:
    JuneApiType getApiType() const override;

private:
    NoApiContext(Instance* instance, JuneApiContextDescriptor const* descriptor);
};

} // namespace june
