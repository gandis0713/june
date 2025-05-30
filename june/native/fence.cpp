#include "fence.h"

#include "instance.h"
#include <spdlog/spdlog.h>

namespace june
{

Fence* Fence::import(Instance* instance, JuneFenceImportDescriptor const* descriptor)
{
    auto fence = new Fence(instance, { .label = std::string(descriptor->label.data, descriptor->label.length) });
    fence->import(descriptor);
    return fence;
}

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
            m_handle = SyncHandle::duplicate(static_cast<SyncHandle::Handle>(syncFDResetDescriptor->syncFD));
            break;
        }
        default:
            spdlog::warn("Unknown fence reset descriptor type: {}", static_cast<uint32_t>(current->sType));
            break;
        }

        current = current->next;
    }
}

void Fence::import(JuneFenceImportDescriptor const* descriptor)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        switch (current->sType)
        {
        case JuneSType_FenceSyncFDImportDescriptor: {
            auto syncFDImportDescriptor = reinterpret_cast<JuneFenceSyncFDImportDescriptor*>(current);
            if (syncFDImportDescriptor->syncFD == -1)
            {
                spdlog::error("No valid sync FD found in the import descriptor.");
                return;
            }
            m_handle = SyncHandle::duplicate(static_cast<SyncHandle::Handle>(syncFDImportDescriptor->syncFD));
            m_type = FenceType::kFenceType_SyncFD;
            break;
        }
        default:
            spdlog::warn("Unknown fence import descriptor type: {}", static_cast<uint32_t>(current->sType));
            break;
        }

        current = current->next;
    }
}

void Fence::create(JuneFenceCreateDescriptor const* descriptor)
{
    switch (descriptor->type)
    {
    case JuneFenceType_SyncFD:
        m_type = FenceType::kFenceType_SyncFD;
        m_handle = std::move(SyncHandle(-1)); // Initialize with an invalid handle
        break;
    default:
        spdlog::error("Unsupported fence type: {}", static_cast<uint32_t>(descriptor->type));
        m_type = FenceType::kFenceType_None;
        m_handle = -1; // Set to an invalid handle
        break;
    }
}

} // namespace june