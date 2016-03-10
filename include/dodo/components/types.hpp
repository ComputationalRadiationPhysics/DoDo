#pragma once

#include <type_traits>
#include <string>

namespace dodo
{
namespace components
{
namespace types
{

template<typename T>
struct Base
{
    using type = T;
    type value;

    Base() = default;
    Base(T t) : value(t){};
    operator type() const { return value; };
    operator type*() { return &value; };
    virtual bool operator<(const Base& other) const
    {
        return value<other.value;
    }
    virtual ~Base() = default;

};

struct ResourceID : public Base<std::string>
{
    ResourceID() = default;
    ResourceID(std::string s) : Base(s){};
};

struct PortKey : public Base<std::string>
{
    PortKey() = default;
    PortKey(std::string s) : Base(s){};
};
//using ResourceID = std::string;
//using PortKey = std::string;

}
}
}