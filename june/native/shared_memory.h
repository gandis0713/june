#pragma once

#include "june/june.h"

#include "june/memory/raw_memory.h"

#include <memory>
#include <vector>

namespace june
{

class Instance;
class SharedMemory
{
public:
    static SharedMemory* create(Instance* instance, JuneSharedMemoryDescriptor const* descriptor);

public:
    SharedMemory() = delete;
    virtual ~SharedMemory() = default;

    SharedMemory(const SharedMemory&) = delete;
    SharedMemory& operator=(const SharedMemory&) = delete;

public: // June API
public:
    Instance* getInstance() const;
    size_t getSize() const;
    RawMemory* getRawMemory() const;

    void attach(Resource* resource);

private:
    SharedMemory(Instance* instance, std::unique_ptr<RawMemory> rawMemory, JuneSharedMemoryDescriptor const* descriptor);

private:
    Instance* m_instance{ nullptr };
    std::unique_ptr<RawMemory> m_rawMemory{ nullptr };
    const JuneSharedMemoryDescriptor m_descriptor;

    std::vector<Resource*> m_attachedApiResources{};
};

} // namespace june