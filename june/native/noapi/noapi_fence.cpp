#include "noapi_fence.h"

#include "june/native/vulkan/vulkan_fence.h"
#include "noapi_context.h"

#include <spdlog/spdlog.h>

namespace june
{

NoApiFence* NoApiFence::create(NoApiContext* context, JuneFenceDescriptor const* descriptor)
{
    return new NoApiFence(context, descriptor);
}

NoApiFence::NoApiFence(NoApiContext* context, JuneFenceDescriptor const* descriptor)
    : Fence(context, descriptor)
{
    m_type = FenceType::kFenceType_FD;

    refresh();
}

void NoApiFence::refresh()
{
    createFd();
}

int NoApiFence::getFd() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_signalFd;
}

void NoApiFence::createFd()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // TODO
}
} // namespace june