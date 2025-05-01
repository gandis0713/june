#include "instance.h"
#include "gles/gles_context.h"
#include "june/native/shared_memory.h"
#include "vulkan/vulkan_context.h"

namespace june
{

Instance* Instance::create(JuneInstanceDescriptor const* descriptor)
{
    return new Instance(descriptor);
}

Instance::Instance(JuneInstanceDescriptor const* descriptor)
    : Object(std::string(descriptor->label.data, descriptor->label.length))
{
}

ApiContext* Instance::createApiContext(JuneApiContextDescriptor const* descriptor)
{
    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        switch (current->sType)
        {
        case JuneSType_VulkanContext: {
            return VulkanContext::create(this, descriptor);
        }
        break;
        case JuneSType_GLESContext: {
            return GLESContext::create(this, descriptor);
        }
        default:
            throw std::runtime_error("Unsupported type");
        }

        current = current->next;
    }

    return nullptr;
}

SharedMemory* Instance::createSharedMemory(JuneSharedMemoryDescriptor const* descriptor)
{
    return SharedMemory::create(this, descriptor);
}

} // namespace june