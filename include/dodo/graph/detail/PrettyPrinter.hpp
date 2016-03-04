#pragma once

namespace dodo
{
namespace graph
{
namespace detail
{

template<typename T>
struct PrettyPrinter
{
};


template<>
struct PrettyPrinter<components::types::ResourceID>
{
    static std::string f(const components::types::ResourceID& id)
    {
        std::ostringstream oss;
        oss << "prettyID:" << id;
        return oss.str();
    }
};


}
}
}

