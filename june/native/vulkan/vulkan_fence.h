#pragma once

#include "june/june.h"
#include "june/native/fence.h"

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
    void wait() override;
};

} // namespace june