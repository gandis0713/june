#pragma once

#include "june/june.h"
#include "june/native/fence.h"
#include "vulkan_api.h"

#include <mutex>
#include <unordered_map>

namespace june
{

class VulkanContext;
class VulkanFence : public Fence
{
public:
    static VulkanFence* create(VulkanContext* context, JuneFenceCreateDescriptor const* descriptor);

public:
    VulkanFence() = delete;
    VulkanFence(VulkanContext* context, JuneFenceCreateDescriptor const* descriptor);
    ~VulkanFence() override = default;

    VulkanFence(const VulkanFence&) = delete;
    VulkanFence& operator=(const VulkanFence&) = delete;

public: // June API
    void reset(JuneFenceResetDescriptor const* descriptor) override;
    void exportFence(JuneFenceExportDescriptor const* descriptor) override;

public:
    void refresh() override;
    int getFd() const override;

public:
    VkSemaphore getVkSemaphore() const;

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