#pragma once

#include "june/june.h"
#include "june/native/vulkan/vulkan_api_memory.h"

#include "vulkan_api.h"

namespace june
{

class VulkanApiContext;
class VulkanAHardwareBufferApiMemory : public VulkanApiMemory
{
public:
    static VulkanAHardwareBufferApiMemory* create(VulkanApiContext* context, JuneApiMemoryDescriptor const* descriptor);

public:
    VulkanAHardwareBufferApiMemory() = delete;
    VulkanAHardwareBufferApiMemory(VulkanApiContext* context, JuneApiMemoryDescriptor const* descriptor);
    ~VulkanAHardwareBufferApiMemory() override = default;

    VulkanAHardwareBufferApiMemory(const VulkanAHardwareBufferApiMemory&) = delete;
    VulkanAHardwareBufferApiMemory& operator=(const VulkanAHardwareBufferApiMemory&) = delete;

public: // June API
    void beginAccess(JuneApiMemoryBeginAccessDescriptor const* descriptor) override;
    void endAccess(JuneApiMemoryEndAccessDescriptor const* descriptor) override;
    void* createResource(JuneResourceDescriptor const* descriptor) override;

private:
    int32_t initialize();
};

} // namespace june