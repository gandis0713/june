#include "noapi_context.h"

#include "june/common/assert.h"
#include "june/native/gles/gles_fence.h"
#include "june/native/vulkan/vulkan_fence.h"
#include "noapi_fence.h"

#include "june/native/shared_memory.h"

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace june
{

ApiContext* NoApiContext::create(Instance* instance, JuneApiContextDescriptor const* descriptor)
{
    return new NoApiContext(instance, descriptor);
}

NoApiContext::NoApiContext(Instance* instance, JuneApiContextDescriptor const* descriptor)
    : ApiContext(instance, descriptor)
{
    // It assumes that the descriptor is valid and has been validated before this point.
    auto NoApiDescriptor = reinterpret_cast<JuneNoApiContextDescriptor const*>(descriptor->nextInChain);
}

NoApiContext::~NoApiContext()
{
}

void NoApiContext::createResource(JuneResourceCreateDescriptor const* descriptor)
{
    // nothing do to
}

Fence* NoApiContext::createFence(JuneFenceCreateDescriptor const* descriptor)
{
    return NoApiFence::create(this, descriptor);
}

JuneApiType NoApiContext::getApiType() const
{
    return JuneApiType_GLES;
}

} // namespace june
