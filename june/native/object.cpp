#include "object.h"

namespace june
{

Object::Object(const std::string& name)
    : m_name(name)
{
}

void Object::setName(const std::string& name)
{
    m_name = name;
}

const std::string& Object::getName() const
{
    return m_name;
}

} // namespace june