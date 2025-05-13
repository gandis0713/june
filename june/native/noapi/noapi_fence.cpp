#include "noapi_fence.h"

#include "june/native/vulkan/vulkan_fence.h"
#include "noapi_context.h"

#include <spdlog/spdlog.h>

namespace june
{

NoApiFence* NoApiFence::create(NoApiContext* context, JuneFenceCreateDescriptor const* descriptor)
{
    return new NoApiFence(context, descriptor);
}

NoApiFence::NoApiFence(NoApiContext* context, JuneFenceCreateDescriptor const* descriptor)
    : Fence(context, descriptor)
{
    m_type = FenceType::kFenceType_SyncFD;
}

void NoApiFence::reset(JuneFenceResetDescriptor const* descriptor)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_signalFd != -1)
    {
        close(m_signalFd);
        m_signalFd = -1;
    }

    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        switch (current->sType)
        {
        case JuneSType_FenceSyncFDResetDescriptor: {
            auto fenceSyncFDDescriptor = reinterpret_cast<JuneFenceSyncFDResetDescriptor const*>(current);
            m_signalFd = fenceSyncFDDescriptor->syncFD;
            break;
        }
        default:
            break;
        }

        current = current->next;
    }
}

int NoApiFence::getSyncFD() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_signalFd == -1)
    {
        return m_signalFd;
    }

    int dupFd = dup(m_signalFd);
    if (dupFd == -1)
    {
        spdlog::error("Failed to duplicate sync FD: {}", m_signalFd);
    }

    return dupFd;
}

} // namespace june