#pragma once

#include "june/june.h"
#include "june/native/vulkan/vulkan_resource.h"

#include "vulkan_api.h"

namespace june
{

class VulkanApiContext;
class VulkanAHardwareBufferResource : public VulkanResource
{
public:
    static VulkanAHardwareBufferResource* create(VulkanApiContext* context, JuneResourceDescriptor const* descriptor);

public:
    VulkanAHardwareBufferResource() = delete;
    VulkanAHardwareBufferResource(VulkanApiContext* context, JuneResourceDescriptor const* descriptor);
    ~VulkanAHardwareBufferResource() override = default;

    VulkanAHardwareBufferResource(const VulkanAHardwareBufferResource&) = delete;
    VulkanAHardwareBufferResource& operator=(const VulkanAHardwareBufferResource&) = delete;

public: // June API
    void beginAccess(JuneResourceBeginAccessDescriptor const* descriptor) override;
    void endAccess(JuneResourceEndAccessDescriptor const* descriptor) override;
    void* getResource(JuneGetResourceDescriptor const* descriptor) override;

private:
    int32_t initialize();
    bool createResource();

private:
    VkImage m_image{ VK_NULL_HANDLE };
};

} // namespace june