#include "gles_api_memory.h"

#include "gles_api.h"
#include "gles_api_context.h"
#include "june/native/shared_memory.h"

namespace june
{

GLESApiMemory::GLESApiMemory(GLESApiContext* context, JuneApiMemoryDescriptor const* descriptor)
    : ApiMemory(context, descriptor)
{
}

} // namespace june