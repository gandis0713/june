#pragma once

#include "june/june.h"
#include "june/native/shared_memory.h"

namespace june
{

class GLESContext;
class GLESBufferMemory : public BufferMemory
{
public:
    static GLESBufferMemory* create(GLESContext* context, JuneBufferMemoryDescriptor const* descriptor);

public:
    GLESBufferMemory() = delete;
    GLESBufferMemory(GLESContext* context, JuneBufferMemoryDescriptor const* descriptor);
    ~GLESBufferMemory() override = default;

    GLESBufferMemory(const GLESBufferMemory&) = delete;
    GLESBufferMemory& operator=(const GLESBufferMemory&) = delete;

public: // June API
    Buffer* createBuffer(JuneBufferDescriptor const* descriptor) override;

public:
    Context* getContext() const override;

private:
    GLESContext* m_context;
    const JuneBufferMemoryDescriptor m_descriptor;
};

} // namespace june