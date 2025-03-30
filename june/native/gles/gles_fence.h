#pragma once

#include "june/june.h"
#include "june/native/fence.h"

namespace june
{

class GLESApiContext;
class GLESFence : public Fence
{
public:
    static GLESFence* create(GLESApiContext* context, JuneFenceDescriptor const* descriptor);

public:
    GLESFence() = delete;
    GLESFence(GLESApiContext* context, JuneFenceDescriptor const* descriptor);
    ~GLESFence() override = default;

    GLESFence(const GLESFence&) = delete;
    GLESFence& operator=(const GLESFence&) = delete;

public: // June API
};

} // namespace june