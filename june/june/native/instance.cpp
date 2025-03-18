#include "instance.h"
// #include "gles/gles_context.h"
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
        // case JuneSType_GLESApiContext: {
        //     JuneGLESApiContextDescriptor const* glesDescriptor = reinterpret_cast<JuneGLESApiContextDescriptor const*>(current);
        //     return GLESContext::create(this, glesDescriptor);
        // }
        default:
            throw std::runtime_error("Unsupported type");
        }

        current = current->next;
    }

    return nullptr;
}

} // namespace june