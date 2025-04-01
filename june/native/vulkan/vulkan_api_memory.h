#pragma once

#include "june/june.h"
#include "june/native/api_memory.h"

namespace june
{

class VulkanApiContext;
class VulkanApiMemory : public ApiMemory
{
public:
    VulkanApiMemory() = delete;
    ~VulkanApiMemory() override = default;

    VulkanApiMemory(const VulkanApiMemory&) = delete;
    VulkanApiMemory& operator=(const VulkanApiMemory&) = delete;

protected:
    VulkanApiMemory(VulkanApiContext* context, JuneApiMemoryDescriptor const* descriptor);
};

} // namespace june