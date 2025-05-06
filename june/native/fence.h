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
    kFenceType_SyncFD,
};

class ApiContext;
class Fence : public Object
{
public:
    Fence() = delete;
    virtual ~Fence() = default;

    Fence(const Fence&) = delete;
    Fence& operator=(const Fence&) = delete;

public: // June API
    virtual void reset(JuneFenceResetDescriptor const* descriptor) = 0;
    virtual void exportFence(JuneFenceExportDescriptor const* descriptor) = 0;

public:
    virtual void refresh() = 0;
    virtual int getFd() const = 0;

public:
    FenceType getType() const;

protected:
    Fence(ApiContext* context, JuneFenceCreateDescriptor const* descriptor);

protected:
    ApiContext* m_context{ nullptr };
    const JuneFenceCreateDescriptor m_descriptor;

    FenceType m_type{ FenceType::kFenceType_None };
};

} // namespace june