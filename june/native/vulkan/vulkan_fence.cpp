#include "vulkan_fence.h"

#include "vulkan_api_context.h"

#include "june/native/gles/gles_fence.h"

#include <spdlog/spdlog.h>

namespace june
{

VulkanFence* VulkanFence::create(VulkanApiContext* context, JuneFenceDescriptor const* descriptor)
{
    return new VulkanFence(context, descriptor);
}

VulkanFence::VulkanFence(VulkanApiContext* context, JuneFenceDescriptor const* descriptor)
    : Fence(context, descriptor)
{
    m_type = FenceType::kFenceType_Vulkan;
}

void VulkanFence::begin()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    spdlog::debug(__func__);

    // wait input fences.??
}

// TODO: store each handle types.
void VulkanFence::end()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    spdlog::debug(__func__);

    if (m_signalSemaphore != VK_NULL_HANDLE)
        return;

    VkExportSemaphoreCreateInfo exportSemCreateInfo = {};
    exportSemCreateInfo.sType = VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO;
    exportSemCreateInfo.handleTypes = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_SYNC_FD_BIT;

    VkSemaphoreTypeCreateInfo timelineCreateInfo;
    timelineCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
    timelineCreateInfo.pNext = &exportSemCreateInfo;
    timelineCreateInfo.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
    timelineCreateInfo.initialValue = 0;

    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = &exportSemCreateInfo;

    auto vulkanApiContext = reinterpret_cast<VulkanApiContext*>(m_context);
    VkDevice device = vulkanApiContext->getVkDevice();
    const auto& vkAPI = vulkanApiContext->vkAPI;

    VkResult result = vkAPI.CreateSemaphore(device, &semaphoreCreateInfo, nullptr, &m_signalSemaphore);
    if (result != VK_SUCCESS)
    {
        spdlog::error("Failed to create Vulkan semaphore: {}", static_cast<uint32_t>(result));
        return;
    }

    signal();
}

// TODO: return each handle types.
int VulkanFence::getFenceFd() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    auto vulkanApiContext = reinterpret_cast<VulkanApiContext*>(m_context);
    VkDevice device = vulkanApiContext->getVkDevice();
    const auto& vkAPI = vulkanApiContext->vkAPI;

    VkSemaphoreGetFdInfoKHR getFdInfo = {};
    getFdInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_GET_FD_INFO_KHR;
    getFdInfo.semaphore = m_signalSemaphore;
    getFdInfo.handleType = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_SYNC_FD_BIT;

    int fenceFd = -1;
    VkResult result = vkAPI.GetSemaphoreFdKHR(device, &getFdInfo, &fenceFd);
    spdlog::trace("Get semaphore fd: {}", fenceFd);
    if (result != VK_SUCCESS || fenceFd < 0)
    {
        spdlog::error("Failed to get Vulkan semaphore fd: {}", static_cast<uint32_t>(result));
    }

    return fenceFd;
}

void VulkanFence::updated(Fence* fence)
{
    auto vulkanApiContext = reinterpret_cast<VulkanApiContext*>(m_context);
    const auto& vkAPI = vulkanApiContext->vkAPI;

    switch (fence->getType())
    {
    case FenceType::kFenceType_GLES: {
        if (false) // TODO: do not destroy old semaphore. because it may be inflight. and check destory time.
        {
            VkSemaphore oldSemaphore = m_waitSemaphores.contains(fence) ? m_waitSemaphores[fence] : VK_NULL_HANDLE;
            if (oldSemaphore)
            {
                vkAPI.DestroySemaphore(vulkanApiContext->getVkDevice(), oldSemaphore, nullptr);
                m_waitSemaphores.erase(fence);
            }
        }

        // VkExportSemaphoreCreateInfo exportSemCreateInfo = {};
        // exportSemCreateInfo.sType = VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO;
        // exportSemCreateInfo.handleTypes = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_SYNC_FD_BIT;

        VkSemaphoreCreateInfo semaphoreCreateInfo = {};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        // semaphoreCreateInfo.pNext = &exportSemCreateInfo;
        semaphoreCreateInfo.pNext = nullptr;

        VkSemaphore semaphore{ VK_NULL_HANDLE };
        VkResult result = vkAPI.CreateSemaphore(vulkanApiContext->getVkDevice(), &semaphoreCreateInfo, nullptr, &semaphore);
        if (result != VK_SUCCESS)
        {
            spdlog::error("Failed to create Vulkan semaphore: {}", static_cast<uint32_t>(result));
            return;
        }

        VkImportSemaphoreFdInfoKHR importSemaphoreFdInfo = {};
        importSemaphoreFdInfo.sType = VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_FD_INFO_KHR;
        importSemaphoreFdInfo.semaphore = semaphore;
        importSemaphoreFdInfo.flags = 0; // or VK_SEMAPHORE_IMPORT_TEMPORARY_BIT
        importSemaphoreFdInfo.handleType = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_SYNC_FD_BIT;
        importSemaphoreFdInfo.fd = static_cast<GLESFence*>(fence)->getFenceFd();

        result = vkAPI.ImportSemaphoreFdKHR(vulkanApiContext->getVkDevice(), &importSemaphoreFdInfo);
        if (result != VK_SUCCESS)
        {
            spdlog::error("Failed to import Vulkan semaphore fd: {}", static_cast<uint32_t>(result));
            return;
        }

        m_waitSemaphores[fence] = semaphore;
    }
    break;
    case FenceType::kFenceType_Vulkan: {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreCreateInfo.pNext = nullptr;

        VkSemaphore semaphore{ VK_NULL_HANDLE };
        VkResult result = vkAPI.CreateSemaphore(vulkanApiContext->getVkDevice(), &semaphoreCreateInfo, nullptr, &semaphore);
        if (result != VK_SUCCESS)
        {
            spdlog::error("Failed to create Vulkan semaphore: {}", static_cast<uint32_t>(result));
            return;
        }

        VkImportSemaphoreFdInfoKHR importSemaphoreFdInfo = {};
        importSemaphoreFdInfo.sType = VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_FD_INFO_KHR;
        importSemaphoreFdInfo.semaphore = semaphore;
        importSemaphoreFdInfo.flags = 0; // or VK_SEMAPHORE_IMPORT_TEMPORARY_BIT
        importSemaphoreFdInfo.handleType = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_SYNC_FD_BIT;
        importSemaphoreFdInfo.fd = static_cast<VulkanFence*>(fence)->getFenceFd();

        m_waitSemaphores[fence] = semaphore;
    }
    break;
    default:
        // TODO: handle this case.
        spdlog::error("Unknown fence type: {}", static_cast<uint32_t>(fence->getType()));
        break;
    }
}

} // namespace june