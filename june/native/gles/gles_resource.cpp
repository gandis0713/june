#include "gles_resource.h"

#include "gles_api.h"
#include "gles_api_context.h"
#include "june/native/shared_memory.h"

namespace june
{

GLESResource::GLESResource(GLESApiContext* context, JuneResourceDescriptor const* descriptor)
    : Resource(context, descriptor)
{
}

} // namespace june