#include "vulkan_resource.h"

#include "vulkan_api_context.h"

namespace june
{

VulkanResource::VulkanResource(VulkanApiContext* context, JuneResourceDescriptor const* descriptor)
    : Resource(context, descriptor)
{
}

} // namespace june