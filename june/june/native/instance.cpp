#include "instance.h"
#include "vulkan/vulkan_context.h"

namespace june
{

Instance* Instance::create(JuneInstanceDescriptor const* descriptor)
{
    return new Instance(descriptor);
}

Instance::Instance(JuneInstanceDescriptor const* descriptor)
{
}

Context* Instance::createApiContext(JuneApiContextDescriptor const* descriptor)
{
    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        switch (current->sType)
        {
        case JuneSType_VulkanApiContext: {
            JuneVulkanApiContextDescriptor const* vulkanDescriptor = reinterpret_cast<JuneVulkanApiContextDescriptor const*>(current);
            return VulkanContext::create(this, vulkanDescriptor);
        }
        break;
        default:
            throw std::runtime_error("Unsupported type");
        }

        current = current->next;
    }

    return nullptr;
}

} // namespace june