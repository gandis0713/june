#include "instance.h"

namespace june
{

std::unique_ptr<Instance> Instance::create(JuneInstanceDescriptor const* descriptor)
{
    return std::unique_ptr<Instance>(new Instance(descriptor));
}

} // namespace june