#include "ahardwarebuffer_memory.h"

#include "june/native/api_context.h"

#include <spdlog/spdlog.h>

namespace june
{

std::unique_ptr<AHardwareBufferMemory> AHardwareBufferMemory::import(const AHardwareBufferImportDescriptor& descriptor)
{
    return std::unique_ptr<AHardwareBufferMemory>(new AHardwareBufferMemory(descriptor));
}

std::unique_ptr<AHardwareBufferMemory> AHardwareBufferMemory::create(const AHardwareBufferCreateDescriptor& descriptor)
{
    return std::unique_ptr<AHardwareBufferMemory>(new AHardwareBufferMemory(descriptor));
}

AHardwareBufferMemory::AHardwareBufferMemory(const AHardwareBufferImportDescriptor& descriptor)
    : RawMemory(RawMemoryDescriptor{
          .type = RawMemoryType::kAHardwareBuffer,
          .hasOwnership = false,
      })
    , m_aHardwareBuffer(descriptor.aHardwareBuffer)
{
    AHardwareBuffer_describe(m_aHardwareBuffer, &m_aHardwareBufferDesc);
}

AHardwareBufferMemory::AHardwareBufferMemory(const AHardwareBufferCreateDescriptor& descriptor)
    : RawMemory(RawMemoryDescriptor{
          .type = RawMemoryType::kAHardwareBuffer,
          .hasOwnership = true,
      })
    , m_aHardwareBufferDesc(*descriptor.aHardwareBufferDesc)
{
    int result = AHardwareBuffer_allocate(&m_aHardwareBufferDesc, &m_aHardwareBuffer);
    if (result != 0)
    {
        spdlog::error("Failed to allocate AHardwareBuffer: {}", result);
    }
}

AHardwareBufferMemory::~AHardwareBufferMemory()
{
    if (m_descriptor.hasOwnership)
        AHardwareBuffer_release(m_aHardwareBuffer);
}

AHardwareBuffer* AHardwareBufferMemory::getAHardwareBuffer() const
{
    return m_aHardwareBuffer;
}

} // namespace june