#pragma once

#include "june/common/sync_handle.h"
#include "june/june.h"
#include "object.h"

#include <mutex>
#include <sigslot/signal.hpp>
#include <string>
#include <unordered_set>
#include <vector>

namespace june
{

enum class FenceType
{
    kFenceType_None = 0,
    kFenceType_SyncFD,
    kFenceType_VkSemaphoreOpaqueFD,
};

struct FenceDescriptor
{
    std::string label;
};

class Instance;
class Fence : public Object
{
public:
    static Fence* import(Instance* instance, JuneFenceImportDescriptor const* descriptor);
    static Fence* create(Instance* instance, JuneFenceCreateDescriptor const* descriptor);

public:
    Fence() = delete;
    virtual ~Fence() = default;

    Fence(const Fence&) = delete;
    Fence& operator=(const Fence&) = delete;

public: // June API
    void reset(JuneFenceResetDescriptor const* descriptor);

public:
    FenceType getType() const;
    SyncHandle& getHandle();

protected:
    Fence(Instance* instance, const FenceDescriptor& descriptor);
    void import(JuneFenceImportDescriptor const* descriptor);
    void create(JuneFenceCreateDescriptor const* descriptor);

protected:
    mutable std::mutex m_mutex;

    Instance* m_instance{ nullptr };
    FenceType m_type{ FenceType::kFenceType_None };
    SyncHandle m_handle{};
};

} // namespace june