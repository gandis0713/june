#pragma once

#include "june/june.h"

namespace june
{

class Fence
{
public:
    Fence() = delete;
    virtual ~Fence() = default;

    Fence(const Fence&) = delete;
    Fence& operator=(const Fence&) = delete;

public: // June API
protected:
    Fence(JuneFenceDescriptor const* descriptor);

protected:
    const JuneFenceDescriptor m_descriptor;
};

} // namespace june