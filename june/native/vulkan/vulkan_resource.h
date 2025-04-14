#pragma once

#include "june/june.h"
#include "june/native/resource.h"

namespace june
{

class VulkanApiContext;
class VulkanResource : public Resource
{
public:
    VulkanResource() = delete;
    ~VulkanResource() override = default;

    VulkanResource(const VulkanResource&) = delete;
    VulkanResource& operator=(const VulkanResource&) = delete;

protected:
    VulkanResource(VulkanApiContext* context, JuneResourceDescriptor const* descriptor);
};

} // namespace june