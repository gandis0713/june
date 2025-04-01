#pragma once

#include "june/june.h"

namespace june
{

class ApiContext;
class Fence
{
public:
    Fence() = delete;
    virtual ~Fence() = default;

    Fence(const Fence&) = delete;
    Fence& operator=(const Fence&) = delete;

protected:
    Fence(ApiContext* context, JuneFenceDescriptor const* descriptor);

protected:
    ApiContext* m_context{ nullptr };
    const JuneFenceDescriptor m_descriptor;
};

} // namespace june