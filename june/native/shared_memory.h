#pragma once

#include "june/june.h"
#include "june/memory/raw_memory.h"
#include "object.h"

#include <memory>
#include <mutex>
#include <unordered_set>

namespace june
{

class Instance;
class ApiContext;
class SharedMemory : public Object
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
    void lock(ApiContext* apiContext);
    void unlock(ApiContext* apiContext);
    void attach(ApiContext* apiContext);
    void detach(ApiContext* apiContext);

private:
    SharedMemory(Instance* instance, std::unique_ptr<RawMemory> rawMemory, JuneSharedMemoryDescriptor const* descriptor);

private:
    Instance* m_instance{ nullptr };
    std::unique_ptr<RawMemory> m_rawMemory{ nullptr };
    const JuneSharedMemoryDescriptor m_descriptor;

    std::unordered_set<ApiContext*> m_attachedApiContext{};

    ApiContext* m_ownerApiContext{ nullptr };
    mutable std::mutex m_mutex{};
};

} // namespace june