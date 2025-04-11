#pragma once

#include "june/june.h"

#include <sigslot/signal.hpp>
#include <unordered_set>
#include <vector>

namespace june
{

enum class FenceType
{
    kFenceType_None = 0,
    kFenceType_GLES,
    kFenceType_Vulkan,
};

class ApiContext;
class Fence
{
public:
    Fence() = delete;
    virtual ~Fence() = default;

    Fence(const Fence&) = delete;
    Fence& operator=(const Fence&) = delete;

public:
    virtual void begin() = 0;
    virtual void end() = 0;

public:
    void connect(Fence* inputFence);
    void addInput(Fence* fence);
    void removeInput(Fence* fence);
    void removeInputAll();
    void addOutput(Fence* fence);
    void removeOutput(Fence* fence);
    void removeOutputAll();
    FenceType getType() const;

protected:
    Fence(ApiContext* context, JuneFenceDescriptor const* descriptor);

protected:
    void signal();
    void slot(Fence* fence);
    virtual void updated(Fence* fence) = 0;

protected:
    ApiContext* m_context{ nullptr };
    const JuneFenceDescriptor m_descriptor;

    FenceType m_type{ FenceType::kFenceType_None };
    sigslot::signal<Fence*> m_signal;

    std::unordered_set<Fence*> m_inputs{};
    std::unordered_set<Fence*> m_outputs{};
};

} // namespace june