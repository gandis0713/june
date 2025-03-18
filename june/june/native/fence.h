#pragma once

#include "june/june.h"

namespace june
{

class Buffer;
class Texture;
class Fence
{
public:
    virtual ~Fence() = default;

    Fence(const Fence&) = delete;
    Fence& operator=(const Fence&) = delete;

public: // June API
public:
    virtual Buffer* getBuffer() const = 0;   // TODO
    virtual Texture* getTexture() const = 0; // TODO

protected:
    Fence() = default;
};

} // namespace june