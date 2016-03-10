#pragma once

#include <type_traits>
#include <string>

namespace dodo
{
namespace components
{
namespace types
{

//using ResourceID = std::string;
//using PortKey = std::string;

template<typename T>
struct Base
{
    using value_type = T;
    value_type value;

    Base() = default;
    Base(T t) : value(t){};
    operator value_type() const { return value; };
//    operator value_type*() { return &value; };
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

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Base<T>& base)
{
    stream << base.value;
    return stream;
}


}
}
}

//namespace boost {
//
//template<>
//std::string lexical_cast(const dodo::components::types::ResourceID& arg) { return arg.value; }
//
//template<>
//std::string lexical_cast(const dodo::components::types::PortKey& arg) { return arg.value; }
//
//}
