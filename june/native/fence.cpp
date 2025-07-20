#include "fence.h"

#include "instance.h"
#include <spdlog/spdlog.h>

namespace june
{

Fence* Fence::create(Instance* instance, JuneFenceCreateDescriptor const* descriptor)
{
    auto fence = new Fence(instance, { .label = std::string(descriptor->label.data, descriptor->label.length) });
    fence->create(descriptor);
    return fence;
}

Fence::Fence(Instance* instance, const FenceDescriptor& descriptor)
    : Object(descriptor.label)
    , m_instance(instance)
{
}

FenceType Fence::getType() const
{
    return m_type;
}

SyncHandle& Fence::getHandle()
{
    return m_handle;
}

void Fence::reset(JuneFenceResetDescriptor const* descriptor)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_handle.close();

    JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        switch (current->sType)
        {
        case JuneSType_FenceSyncFDResetDescriptor: {
            auto syncFDResetDescriptor = reinterpret_cast<JuneFenceSyncFDResetDescriptor*>(current);
            if (syncFDResetDescriptor->syncFD == -1)
            {
                spdlog::error("Invalid sync FD for resetting.");
                return;
            }
            spdlog::trace("{} Resetting sync FD: {}", getName(), syncFDResetDescriptor->syncFD);
            m_handle = SyncHandle::duplicate(syncFDResetDescriptor->syncFD);
            break;
        }
        default:
            spdlog::warn("Unknown fence reset descriptor type: {}", static_cast<uint32_t>(current->sType));
            break;
        }

        current = current->next;
    }
}

void Fence::create(JuneFenceCreateDescriptor const* descriptor)
{
    JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        switch (current->sType)
        {
#if defined(__ANDROID__) || defined(ANDROID)
        case JuneSType_FenceSyncFDImportDescriptor: {
            auto importDesc = reinterpret_cast<JuneFenceSyncFDImportDescriptor*>(current);
            if (importDesc->syncFD == -1)
            {
                spdlog::error("Invalid sync FD for fence creation.");
                return;
            }
            m_handle = SyncHandle::duplicate(importDesc->syncFD);
            m_type = FenceType::kFenceType_SyncFD;
            break;
        }
        case JuneSType_FenceEGLSyncImportDescriptor: {
            // TODO: Implement EGL sync import
            spdlog::error("EGL sync import not implemented yet.");
            break;
        }
#endif
        default:
            spdlog::warn("Unknown fence create descriptor type: {}", static_cast<uint32_t>(current->sType));
            break;
        }

        current = current->next;
    }
}

} // namespace june