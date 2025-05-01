#pragma once

#include "june/june.h"
#include "object.h"

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
    kFenceType_FD,
};

class ApiContext;
class Fence : public Object
{
public:
    Fence() = delete;
    virtual ~Fence() = default;

    Fence(const Fence&) = delete;
    Fence& operator=(const Fence&) = delete;

public:
    virtual void refresh() = 0;

public:
    FenceType getType() const;

protected:
    Fence(ApiContext* context, JuneFenceDescriptor const* descriptor);

protected:
    ApiContext* m_context{ nullptr };
    const JuneFenceDescriptor m_descriptor;

    FenceType m_type{ FenceType::kFenceType_None };
};

} // namespace june