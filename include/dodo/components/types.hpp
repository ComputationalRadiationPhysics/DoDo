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
    using value_type = T;
    value_type value;

    Base() = default;
    Base(T t) : value(t){};
    operator value_type() const { return value; };
//    operator value_type*() { return &value; };
    bool operator<(const Base& other) const
    {
        return value<other.value;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Base& base)
    {
        stream << base.value;
        return stream;
    }

    friend std::string toString(const Base& b)
    {
        std::ostringstream oss;
        oss << b;
        return oss.str();
    }

    virtual ~Base() = default;


};


struct ResourceID : public Base<std::string>
{
    ResourceID() = default;
    ResourceID(Base::value_type s) : Base(s){};
    friend std::string toString(const ResourceID& id)
    {
        std::ostringstream oss;
        oss << "ResourceID: " << id;
        return oss.str();
    }
};

struct PortKey : public Base<std::string>
{
    PortKey() = default;
    PortKey(Base::value_type s) : Base(s){};

    friend std::string toString(const PortKey &id) {
        std::ostringstream oss;
        oss << "Port: " << id;
        return oss.str();
    }
};



}
}
}

