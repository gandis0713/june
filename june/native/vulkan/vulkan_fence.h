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
    void refresh() override;

public:
    VkSemaphore getVkSemaphore() const;
    int getFd() const;

private:
    void createVkSemaphore();
    void createFd();

private:
    mutable std::mutex m_mutex;

    VkSemaphore m_signalSemaphore{ VK_NULL_HANDLE };
    // TODO: use below instead of m_signalSemaphore for each handle type.
    // std::unordered_map<Fence*, VkSemaphore> m_signalSemaphores{};
    int m_signalFd{ -1 };
};

} // namespace june