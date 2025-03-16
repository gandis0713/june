#pragma once

#include "june/june.h"
#include "june/native/buffer.h"

namespace june
{

class GLESContext;
class GLESBufferMemory;
class GLESBuffer : public Buffer
{
public:
    static GLESBuffer* create(GLESBufferMemory* memory, JuneBufferDescriptor const* descriptor);

public:
    GLESBuffer() = delete;
    GLESBuffer(GLESBufferMemory* memory, JuneBufferDescriptor const* descriptor);
    ~GLESBuffer() override = default;

    GLESBuffer(const GLESBuffer&) = delete;
    GLESBuffer& operator=(const GLESBuffer&) = delete;

public: // June API
public:
    Context* getContext() const override;
    BufferMemory* getMemory() const override;

private:
    GLESBufferMemory* m_memory;
    const JuneBufferDescriptor m_descriptor;
};

} // namespace june