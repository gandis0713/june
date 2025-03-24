#include "vulkan_shared_memory.h"

#if defined(__ANDROID__) || defined(ANDROID)
#include "june/memory/ahardwarebuffer_memory.h"
#endif
#include "vulkan_api_context.h"

namespace june
{

VulkanSharedMemory* VulkanSharedMemory::create(VulkanApiContext* context, JuneSharedMemoryDescriptor const* descriptor)
{
    RawMemoryDescriptor rawMemoryDescriptor;
    rawMemoryDescriptor.type = RawMemoryType::kAHardwareBuffer;
    rawMemoryDescriptor.width = descriptor->width;
    rawMemoryDescriptor.height = descriptor->height;
    rawMemoryDescriptor.layers = descriptor->layers;

    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        switch (current->sType)
        {
#if defined(__ANDROID__) || defined(ANDROID)
        case JuneSType_AHardwareBufferSharedMemory: {
            JuneSharedMemoryAHardwareBufferDescriptor const* ahbDescriptor = reinterpret_cast<JuneSharedMemoryAHardwareBufferDescriptor const*>(current);

            AHardwareBufferMemoryDescriptor ahbMemoryDescriptor;
            ahbMemoryDescriptor.aHardwareBuffer = static_cast<AHardwareBuffer*>(ahbDescriptor->aHardwareBuffer);

            std::unique_ptr<RawMemory> rawMemory = AHardwareBufferMemory::create(this, rawMemoryDescriptor, ahbMemoryDescriptor);
            return new VulkanSharedMemory(context, std::move(rawMemory), descriptor);
        }
        break;
#endif
        default:
            break;
        }

        current = current->next;
    }

    auto defaultMemoryType = RawMemory::getDefaultMemoryType();
    switch (defaultMemoryType)
    {
#if defined(__ANDROID__) || defined(ANDROID)
    case RawMemoryType::kAHardwareBuffer: {
        std::unique_ptr<RawMemory> rawMemory = AHardwareBufferMemory::create(this, rawMemoryDescriptor);
        return new VulkanSharedMemory(context, std::move(rawMemory), descriptor);
    }
    break;
#endif
    default:
        break;
    }

    return new VulkanSharedMemory(context, nullptr, descriptor);
}

VulkanSharedMemory::VulkanSharedMemory(VulkanApiContext* context, std::unique_ptr<RawMemory> rawMemory, JuneSharedMemoryDescriptor const* descriptor)
    : SharedMemory(context, std::move(rawMemory), descriptor)
{
}

void VulkanSharedMemory::beginAccess(JuneBeginAccessDescriptor const* descriptor)
{
    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        switch (current->sType)
        {
        case JuneSType_BeginAccessVulkanTexture: {
            JuneBeginAccessVulkanTextureDescriptor const* vulkanTextureDescriptor = reinterpret_cast<JuneBeginAccessVulkanTextureDescriptor const*>(current);
        }
        break;
        case JuneSType_BeginAccessVulkanBuffer: {
            JuneBeginAccessVulkanBufferDescriptor const* vulkanBufferDescriptor = reinterpret_cast<JuneBeginAccessVulkanBufferDescriptor const*>(current);
        }
        break;
        default:
            break;
        }

        current = current->next;
    }
}

void VulkanSharedMemory::endAccess(JuneEndAccessDescriptor const* descriptor)
{
}

} // namespace june