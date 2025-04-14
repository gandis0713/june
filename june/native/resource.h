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
class Resource
{
public:
    Resource() = delete;
    virtual ~Resource() = default;

    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;

public: // June API
    virtual void beginAccess(JuneResourceBeginAccessDescriptor const* descriptor) = 0;
    virtual void endAccess(JuneResourceEndAccessDescriptor const* descriptor) = 0;
    virtual void* getResource(JuneGetResourceDescriptor const* descriptor) = 0;
    void connect(Resource* inputResource);

public:
    void addInput(Resource* memory);
    void removeInput(Resource* memory);
    void removeInputAll();
    void addOutput(Resource* memory);
    void removeOutput(Resource* memory);
    void removeOutputAll();

    SharedMemory* getSharedMemory() const;
    Fence* getFence() const;

protected:
    void signal();
    void slot(Resource* memory);

protected:
    Resource(ApiContext* context, JuneResourceDescriptor const* descriptor);

protected:
    ApiContext* m_context{ nullptr };
    const JuneResourceDescriptor m_descriptor;

protected:
    std::unordered_set<Resource*> m_inputs{};
    std::unordered_set<Resource*> m_outputs{};

    Fence* m_fence{ nullptr };
    std::mutex m_accessMutex{};
    sigslot::signal<Resource*> m_signal;
};

} // namespace june