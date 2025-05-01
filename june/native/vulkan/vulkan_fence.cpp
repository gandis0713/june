#include "vulkan_fence.h"

#include "vulkan_context.h"

#include "june/native/gles/gles_fence.h"

#include <spdlog/spdlog.h>

namespace june
{

VulkanFence* VulkanFence::create(VulkanContext* context, JuneFenceDescriptor const* descriptor)
{
    return new VulkanFence(context, descriptor);
}

VulkanFence::VulkanFence(VulkanContext* context, JuneFenceDescriptor const* descriptor)
    : Fence(context, descriptor)
{
    m_type = FenceType::kFenceType_Vulkan;
}

void VulkanFence::refresh()
{
    createVkSemaphore();
    createFd();
}

VkSemaphore VulkanFence::getVkSemaphore() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_signalSemaphore;
}

int VulkanFence::getFd() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_signalFd;
}

void VulkanFence::createVkSemaphore()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_signalSemaphore != VK_NULL_HANDLE)
    {
        // Vulkan semaphore already created
        return;
    }

    VkExportSemaphoreCreateInfo exportSemCreateInfo = {};
    exportSemCreateInfo.sType = VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO;
    exportSemCreateInfo.handleTypes = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_FD_BIT;

    VkSemaphoreTypeCreateInfo timelineCreateInfo;
    timelineCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
    timelineCreateInfo.pNext = &exportSemCreateInfo;
    timelineCreateInfo.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
    timelineCreateInfo.initialValue = 0;

    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = &exportSemCreateInfo;

    auto vulkanContext = reinterpret_cast<VulkanContext*>(m_context);
    VkDevice device = vulkanContext->getVkDevice();
    const auto& vkAPI = vulkanContext->vkAPI;

    VkResult result = vkAPI.CreateSemaphore(device, &semaphoreCreateInfo, nullptr, &m_signalSemaphore);
    if (result != VK_SUCCESS)
    {
        spdlog::error("Failed to create Vulkan semaphore: {}", static_cast<uint32_t>(result));
        return;
    }
}

void VulkanFence::createFd()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_signalFd != -1)
    {
        // Vulkan semaphore fd already created
        return;
    }

    if (m_signalSemaphore == VK_NULL_HANDLE)
    {
        spdlog::error("Vulkan semaphore is not created yet.");
        return;
    }

    auto vulkanContext = reinterpret_cast<VulkanContext*>(m_context);
    VkDevice device = vulkanContext->getVkDevice();
    const auto& vkAPI = vulkanContext->vkAPI;

    VkSemaphoreGetFdInfoKHR getFdInfo = {};
    getFdInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_GET_FD_INFO_KHR;
    getFdInfo.semaphore = m_signalSemaphore;
    getFdInfo.handleType = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_SYNC_FD_BIT;

    VkResult result = vkAPI.GetSemaphoreFdKHR(device, &getFdInfo, &m_signalFd);
    spdlog::trace("Get semaphore fd: {}", m_signalFd);
    if (result != VK_SUCCESS || m_signalFd < 0)
    {
        spdlog::error("Failed to get fd: {}", static_cast<uint32_t>(result));
        return;
    }
}

} // namespace june