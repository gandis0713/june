#pragma once

namespace june
{

class Context;
class Texture
{
public:
    virtual ~Texture() = default;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

public:
    // June API

public:
    virtual Context* getContext() const = 0;

protected:
    Texture() = default;
};

} // namespace june