#pragma once

#include "june/june.h"
#include "raw_memory.h"

#include <android/hardware_buffer.h>

#include <memory>

namespace june
{

struct AHardwareBufferImportDescriptor
{
    AHardwareBuffer* aHardwareBuffer{ nullptr };
};

struct AHardwareBufferCreateDescriptor
{
    AHardwareBuffer_Desc* aHardwareBufferDesc{ nullptr };
};

class ApiContext;
class Buffer;
class AHardwareBufferMemory : public RawMemory
{
public:
    static std::unique_ptr<AHardwareBufferMemory> import(const AHardwareBufferImportDescriptor& descriptor);
    static std::unique_ptr<AHardwareBufferMemory> create(const AHardwareBufferCreateDescriptor& descriptor);

public:
    AHardwareBufferMemory() = delete;
    ~AHardwareBufferMemory() override;

    AHardwareBufferMemory(const AHardwareBufferMemory&) = delete;
    AHardwareBufferMemory& operator=(const AHardwareBufferMemory&) = delete;

public:
    AHardwareBuffer* getAHardwareBuffer() const;

private:
    AHardwareBuffer* m_aHardwareBuffer{ nullptr };
    AHardwareBuffer_Desc m_aHardwareBufferDesc{};

private:
    AHardwareBufferMemory(const AHardwareBufferImportDescriptor& descriptor);
    AHardwareBufferMemory(const AHardwareBufferCreateDescriptor& descriptor);
};

} // namespace june