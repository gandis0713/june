#pragma once

#include "june/june.h"

#include "june/memory/raw_memory.h"

#include <memory>

namespace june
{

class ApiContext;
class Buffer;
class Texture;
class SharedMemory
{
public:
    SharedMemory() = delete;
    virtual ~SharedMemory() = default;

    SharedMemory(const SharedMemory&) = delete;
    SharedMemory& operator=(const SharedMemory&) = delete;

public: // June API
    virtual void beginAccess(JuneBeginAccessDescriptor const* descriptor) = 0;
    virtual void endAccess(JuneEndAccessDescriptor const* descriptor) = 0;

public:
    ApiContext* getContext() const;
    size_t getSize() const;

protected:
    SharedMemory(ApiContext* context, std::unique_ptr<RawMemory> rawMemory, JuneSharedMemoryDescriptor const* descriptor);

protected:
    ApiContext* m_context{ nullptr };
    std::unique_ptr<RawMemory> m_rawMemory{ nullptr };
    const JuneSharedMemoryDescriptor m_descriptor;
};

} // namespace june