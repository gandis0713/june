#pragma once

#include "june/june.h"

namespace june
{

class ApiContext;
class SharedMemory;
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
    ApiContext* createApiContext(JuneApiContextDescriptor const* descriptor);
    SharedMemory* createSharedMemory(JuneSharedMemoryDescriptor const* descriptor);

private:
    Instance(JuneInstanceDescriptor const* descriptor);
};

} // namespace june