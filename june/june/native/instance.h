#pragma once

#include "june/june.h"

#include <memory>

namespace june
{

class Instance
{
public:
    static std::unique_ptr<Instance> create(JuneInstanceDescriptor const* descriptor);

public:
    virtual ~Instance() = default;

    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;

protected:
    Instance() = default;

private:
    Instance(JuneInstanceDescriptor const* descriptor);
};

} // namespace june