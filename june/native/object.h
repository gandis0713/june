#pragma once

#include <string>

namespace june
{

class Object
{
public:
    void setName(const std::string& name);
    const std::string& getName() const;

protected:
    Object() = default;
    Object(const std::string& name);
    virtual ~Object() = default;

protected:
    std::string m_name;
};

} // namespace june