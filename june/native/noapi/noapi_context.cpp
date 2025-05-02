#include "noapi_context.h"

#include "june/common/assert.h"
#include "june/native/gles/gles_fence.h"
#include "june/native/vulkan/vulkan_fence.h"
#include "noapi_fence.h"

#include "june/native/shared_memory.h"

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace june
{

ApiContext* NoApiContext::create(Instance* instance, JuneApiContextDescriptor const* descriptor)
{
    return new NoApiContext(instance, descriptor);
}

NoApiContext::NoApiContext(Instance* instance, JuneApiContextDescriptor const* descriptor)
    : ApiContext(instance, descriptor)
{
    // It assumes that the descriptor is valid and has been validated before this point.
    auto NoApiDescriptor = reinterpret_cast<JuneNoApiContextDescriptor const*>(descriptor->nextInChain);
}

NoApiContext::~NoApiContext()
{
}

void NoApiContext::createResource(JuneResourceDescriptor const* descriptor)
{
    // nothing do to
}

Fence* NoApiContext::createFence(JuneFenceDescriptor const* descriptor)
{
    return NoApiFence::create(this, descriptor);
}

void NoApiContext::beginMemoryAccess(JuneApiContextBeginMemoryAccessDescriptor const* descriptor)
{
    reinterpret_cast<SharedMemory*>(descriptor->sharedMemory)->lock(this);

    const auto fenceCount = descriptor->waitSyncInfo->fenceCount;
    std::vector<int> fds(fenceCount);

    for (auto i = 0; i < fenceCount; ++i)
    {
        auto fence = reinterpret_cast<Fence*>(descriptor->waitSyncInfo->fences[i]);
        switch (fence->getType())
        {
        case FenceType::kFenceType_GLES:
            fds[i] = static_cast<GLESFence*>(fence)->getFd();
            break;
        case FenceType::kFenceType_Vulkan:
            fds[i] = static_cast<VulkanFence*>(fence)->getFd();
            break;
        default:
            spdlog::error("Unknown fence type");
            break;
        }
    }

    JuneChainedStruct* current = descriptor->exportedSyncObject->nextInChain;
    while (current)
    {
        switch (current->sType)
        {
        case JuneSType_SharedMemoryExportedFDSyncObject: {
            auto fdSyncObject = reinterpret_cast<JuneSharedMemoryExportedFDSyncObject*>(current);
            fdSyncObject->fdCount = static_cast<uint32_t>(fds.size());
            fdSyncObject->fds = reinterpret_cast<void*>(fds.data());
            break;
        }
        default:
            break;
        }

        current = current->next;
    }
}

void NoApiContext::endMemoryAccess(JuneApiContextEndMemoryAccessDescriptor const* descriptor)
{
    reinterpret_cast<SharedMemory*>(descriptor->sharedMemory)->unlock(this);
}

JuneApiType NoApiContext::getApiType() const
{
    return JuneApiType_GLES;
}

} // namespace june
