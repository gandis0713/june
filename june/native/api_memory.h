#pragma once

#include "june/june.h"

#include "june/memory/raw_memory.h"

#include <memory>
#include <mutex>
#include <sigslot/signal.hpp>
#include <unordered_set>

namespace june
{

class SharedMemory;
class ApiContext;
class Buffer;
class Texture;
class Fence;
class ApiMemory
{
public:
    ApiMemory() = delete;
    virtual ~ApiMemory() = default;

    ApiMemory(const ApiMemory&) = delete;
    ApiMemory& operator=(const ApiMemory&) = delete;

public: // June API
    virtual void beginAccess(JuneApiMemoryBeginAccessDescriptor const* descriptor) = 0;
    virtual void endAccess(JuneApiMemoryEndAccessDescriptor const* descriptor) = 0;
    virtual void* createResource(JuneResourceDescriptor const* descriptor) = 0;
    void connect(ApiMemory* srcMemory);

public:
    void addInput(ApiMemory* memory);
    void removeInput(ApiMemory* memory);
    void removeInputAll();
    void addOutput(ApiMemory* memory);
    void removeOutput(ApiMemory* memory);
    void removeOutputAll();

    SharedMemory* getSharedMemory() const;
    Fence* getFence() const;

protected:
    void signal();
    void slot();

protected:
    ApiMemory(ApiContext* context, JuneApiMemoryDescriptor const* descriptor);

protected:
    ApiContext* m_context{ nullptr };
    const JuneApiMemoryDescriptor m_descriptor;

protected:
    std::unordered_set<ApiMemory*> m_inputs{};
    std::unordered_set<ApiMemory*> m_outputs{};

    Fence* m_fence{ nullptr };
    std::mutex m_accessMutex{};
    sigslot::signal<> m_signal;
};

} // namespace june