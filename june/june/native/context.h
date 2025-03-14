#pragma once

namespace june
{

class Instance;
class Context
{
public:
    virtual ~Context() = default;

    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

public:
    virtual Instance* getInstance() const = 0;

protected:
    Context() = default;
};
} // namespace june