#include "noapi_context.h"

#include "june/common/assert.h"
#include "june/native/fence.h"
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

void NoApiContext::createResource(JuneResourceCreateDescriptor const* descriptor)
{
    // nothing do to
}

void NoApiContext::exportFence(JuneFenceExportDescriptor const* descriptor)
{
    JuneChainedStruct* current = descriptor->nextInChain;
    Fence* fence = reinterpret_cast<Fence*>(descriptor->fence);

    while (current)
    {
        switch (current->sType)
        {
        case JuneSType_FenceSyncFDExportDescriptor: {
            auto& handle = fence->getHandle();
            auto dupHandle = handle.duplicate();

            int syncFD = dupHandle.getHandle();
            if (syncFD == -1)
            {
                // doesn't need to export
                return;
            }
            spdlog::trace("Duplicated sync FD for NoApiContext name as {}, {}", getName(), syncFD);

            auto syncFDExportDescriptor = reinterpret_cast<JuneFenceSyncFDExportDescriptor*>(current);
            syncFDExportDescriptor->syncFD = syncFD;
            break;
        }
        default:
            break;
        }

        current = current->next;
    }
}

JuneApiType NoApiContext::getApiType() const
{
    return JuneApiType_NoApi;
}

} // namespace june
