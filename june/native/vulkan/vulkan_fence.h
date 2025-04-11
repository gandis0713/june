#pragma once

#include "june/june.h"
#include "june/native/fence.h"
#include "vulkan_api.h"

#include <mutex>
#include <unordered_map>

namespace june
{

class VulkanApiContext;
class VulkanFence : public Fence
{
public:
    static VulkanFence* create(VulkanApiContext* context, JuneFenceDescriptor const* descriptor);

public:
    VulkanFence() = delete;
    VulkanFence(VulkanApiContext* context, JuneFenceDescriptor const* descriptor);
    ~VulkanFence() override = default;

    VulkanFence(const VulkanFence&) = delete;
    VulkanFence& operator=(const VulkanFence&) = delete;

public:
    void begin() override;
    void end() override;

public:
    int getFenceFd() const;

protected:
    void updated(Fence* fence) override;

private:
    mutable std::mutex m_mutex;

    std::unordered_map<Fence*, VkSemaphore> m_waitSemaphores{};
    VkSemaphore m_signalSemaphore{ VK_NULL_HANDLE };
    // TODO: use below instead of m_signalSemaphore for each handle type.
    // std::unordered_map<Fence*, VkSemaphore> m_signalSemaphores{};
};

} // namespace june