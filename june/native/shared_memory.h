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
    static SharedMemory* import(Instance* instance, JuneSharedMemoryImportDescriptor const* descriptor);

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

private:
    SharedMemory(Instance* instance, std::unique_ptr<RawMemory> rawMemory, JuneSharedMemoryImportDescriptor const* descriptor);

private:
    Instance* m_instance{ nullptr };
    std::unique_ptr<RawMemory> m_rawMemory{ nullptr };
};

} // namespace june