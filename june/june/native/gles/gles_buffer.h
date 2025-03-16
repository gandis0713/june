#pragma once

#include "june/june.h"
#include "june/native/buffer.h"

namespace june
{

class GLESContext;
class GLESBuffer : public Buffer
{
public:
    static GLESBuffer* create(GLESContext* context, JuneBufferDescriptor const* descriptor);

public:
    GLESBuffer() = delete;
    GLESBuffer(GLESContext* context, JuneBufferDescriptor const* descriptor);
    ~GLESBuffer() override = default;

    GLESBuffer(const GLESBuffer&) = delete;
    GLESBuffer& operator=(const GLESBuffer&) = delete;

public:
    // June API

public:
    Context* getContext() const override;

private:
    GLESContext* m_context;
    const JuneBufferDescriptor m_descriptor;
};

} // namespace june