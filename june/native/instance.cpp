#include "instance.h"
#include "gles/gles_context.h"
#include "june/native/fence.h"
#include "june/native/shared_memory.h"
#include "noapi/noapi_context.h"
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
        case JuneSType_GLESContext: {
            return GLESContext::create(this, descriptor);
        }
        case JuneSType_NoApiContext: {
            return NoApiContext::create(this, descriptor);
        }
        default:
            throw std::runtime_error("Unsupported context type");
        }

        current = current->next;
    }

    return nullptr;
}

SharedMemory* Instance::importSharedMemory(JuneSharedMemoryImportDescriptor const* descriptor)
{
    return SharedMemory::import(this, descriptor);
}

SharedMemory* Instance::createSharedMemory(JuneSharedMemoryCreateDescriptor const* descriptor)
{
    return SharedMemory::create(this, descriptor);
}

Fence* Instance::importFence(JuneFenceImportDescriptor const* descriptor)
{
    return Fence::import(this, descriptor);
}

Fence* Instance::createFence(JuneFenceCreateDescriptor const* descriptor)
{
    return Fence::create(this, descriptor);
}

} // namespace june