#include "cpu_fence.h"

#include "cpu_context.h"
#include "june/native/vulkan/vulkan_fence.h"

#include <spdlog/spdlog.h>

namespace june
{

CPUFence* CPUFence::create(CPUContext* context, JuneFenceDescriptor const* descriptor)
{
    return new CPUFence(context, descriptor);
}

CPUFence::CPUFence(CPUContext* context, JuneFenceDescriptor const* descriptor)
    : Fence(context, descriptor)
{
    m_type = FenceType::kFenceType_FD;

    refresh();
}

void CPUFence::refresh()
{
    createFd();
}

int CPUFence::getFd() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_signalFd;
}

void CPUFence::createFd()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // TODO
}
} // namespace june