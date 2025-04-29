#pragma once

#include "june/june.h"
#include "vulkan_api.h"

namespace june
{

class VulkanApiContext;
class VulkanAHardwareBufferVkImage
{
public:
    static int create(VulkanApiContext* context, JuneResourceDescriptor const* descriptor);
};

} // namespace june