#pragma once

#include "june/june.h"
#include "june/native/api_memory.h"

namespace june
{

class VulkanApiContext;
class VulkanApiMemory : public ApiMemory
{
public:
    static VulkanApiMemory* create(VulkanApiContext* context, JuneApiMemoryDescriptor const* descriptor);

public:
    VulkanApiMemory() = delete;
    VulkanApiMemory(VulkanApiContext* context, JuneApiMemoryDescriptor const* descriptor);
    ~VulkanApiMemory() override = default;

    VulkanApiMemory(const VulkanApiMemory&) = delete;
    VulkanApiMemory& operator=(const VulkanApiMemory&) = delete;

public: // June API
    void beginAccess(JuneApiMemoryBeginAccessDescriptor const* descriptor) override;
    void endAccess(JuneApiMemoryEndAccessDescriptor const* descriptor) override;
};

} // namespace june