#include "gles_api_memory.h"

#include "gles_api_context.h"

namespace june
{

GLESApiMemory* GLESApiMemory::create(GLESApiContext* context, JuneApiMemoryDescriptor const* descriptor)
{
    return new GLESApiMemory(context, descriptor);
}

GLESApiMemory::GLESApiMemory(GLESApiContext* context, JuneApiMemoryDescriptor const* descriptor)
    : ApiMemory(context, descriptor)
{
}

void GLESApiMemory::beginAccess(JuneApiMemoryBeginAccessDescriptor const* descriptor)
{
    const JuneChainedStruct* current = descriptor->nextInChain;
    while (current)
    {
        current = current->next;
    }
}

void GLESApiMemory::endAccess(JuneApiMemoryEndAccessDescriptor const* descriptor)
{
}

void* GLESApiMemory::createResource(JuneResourceDescriptor const* descriptor)
{
    return nullptr;
}

} // namespace june