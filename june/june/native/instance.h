#pragma once

#include "june/june.h"

namespace june
{

class Context;
class Instance final
{
public:
    static Instance* create(JuneInstanceDescriptor const* descriptor);

public:
    Instance() = delete;
    ~Instance() = default;

    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;

public:
    Context* createApiContext(JuneApiContextDescriptor const* descriptor);

private:
    Instance(JuneInstanceDescriptor const* descriptor);
};

} // namespace june