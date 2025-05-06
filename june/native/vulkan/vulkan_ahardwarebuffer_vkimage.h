#pragma once

#include "june/june.h"
#include "vulkan_api.h"

namespace june
{

class VulkanContext;
class VulkanAHardwareBufferVkImage
{
public:
    static int create(VulkanContext* context, JuneResourceCreateDescriptor const* descriptor);
};

} // namespace june