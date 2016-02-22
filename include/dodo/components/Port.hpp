#pragma once


#include <vector>
#include "Connection.hpp"
#include "Component.hpp"


namespace dodo
{
namespace components
{
    class Port
    {
    public:
        std::weak_ptr<Component> component;
    protected:
        //currently unused
        std::vector<Connection> connections;
    };

}
}
