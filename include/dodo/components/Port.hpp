#pragma once


#include <vector>
#include "Connection.hpp"
#include "dodo/components/meta/Interface.hpp"


namespace dodo
{
namespace components
{
namespace meta{
    class Interface;
}

    class Port
    {
    public:
        std::weak_ptr<meta::Interface> component;
    protected:
        //currently unused
        std::vector<Connection> connections;
    };

}
}
