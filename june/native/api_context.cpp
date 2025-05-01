#include "api_context.h"

#include "instance.h"

namespace june
{

ApiContext::ApiContext(Instance* instance, JuneApiContextDescriptor const* descriptor)
    : Object(std::string(descriptor->label.data, descriptor->label.length))
    , m_instance(instance)
    , m_descriptor(*descriptor)
{
}

Instance* ApiContext::getInstance() const
{
    return m_instance;
}

} // namespace june