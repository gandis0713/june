#include "vulkan_ahardwarebuffer_api_memory.h"

#include "june/memory/ahardwarebuffer_memory.h"
#include "june/native/shared_memory.h"
#include "vulkan_api.h"
#include "vulkan_api_context.h"

#include <spdlog/spdlog.h>

namespace june
{

VulkanAHardwareBufferApiMemory* VulkanAHardwareBufferApiMemory::create(VulkanApiContext* context, JuneApiMemoryDescriptor const* descriptor)
{
    auto apiMemory = new VulkanAHardwareBufferApiMemory(context, descriptor);
    apiMemory->initialize();

    return apiMemory;
}

VulkanAHardwareBufferApiMemory::VulkanAHardwareBufferApiMemory(VulkanApiContext* context, JuneApiMemoryDescriptor const* descriptor)
    : VulkanApiMemory(context, descriptor)
{
}

void VulkanAHardwareBufferApiMemory::beginAccess(JuneApiMemoryBeginAccessDescriptor const* descriptor)
{
    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        current = current->next;
    }
}

void VulkanAHardwareBufferApiMemory::endAccess(JuneApiMemoryEndAccessDescriptor const* descriptor)
{
}

void* VulkanAHardwareBufferApiMemory::createResource(JuneResourceDescriptor const* descriptor)
{
    return nullptr;
}

int32_t VulkanAHardwareBufferApiMemory::initialize()
{
    return -1;
}

} // namespace june