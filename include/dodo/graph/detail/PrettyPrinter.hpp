#pragma once

#include <string>
#include <sstream>

namespace dodo
{
namespace graph
{
namespace detail
{

template<typename T>
std::string prettyPrinter(T &t)
{
    std::ostringstream oss;
    oss << t;
    return oss.str();
};


template<>
std::string prettyPrinter(components::types::ResourceID &id)
{
    std::ostringstream oss;
    oss << "ResourceID: " << id;
    return oss.str();
};
template<>
std::string prettyPrinter(components::types::PortKey &id)
{
    std::ostringstream oss;
    oss << "Port: " << id.value;
    return oss.str();
};

}
}
}

