#pragma once

#include <vector>
#include <memory>

#include "Connection.hpp"
#include "meta/Interface.hpp"


namespace dodo
{
namespace components
{

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
