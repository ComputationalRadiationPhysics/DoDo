#pragma once


#include <vector>
#include "Connection.hpp"
#include "ComponentMetadataInterface.hpp"


namespace dodo
{
namespace components
{
    class ComponentMetadataInterface;

    class Port
    {
    public:
        std::weak_ptr<ComponentMetadataInterface> component;
    protected:
        //currently unused
        std::vector<Connection> connections;
    };

}
}
