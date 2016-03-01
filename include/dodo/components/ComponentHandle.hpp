#pragma once

#include <memory>
#include <exception>
#include <typeinfo>
#include <string>
#include "ComponentMetadataInterface.hpp"
#include "MetadataNetwork.hpp"


namespace dodo
{
namespace components
{

class ComponentHandle
{
public:
    using ResourceID = MetadataNetwork::ResourceID;
    std::weak_ptr<MetadataNetwork> net;
    ResourceID componentID;

    ComponentHandle(std::weak_ptr<MetadataNetwork> net, const ResourceID &&c) :
        net(net),
        componentID(c)
    {}

    auto operator[](std::string portName) const
    {
        auto component = net.lock()->getComponent(componentID);
        if(! component.lock()->hasPort(portName))
        {
            throw std::runtime_error(
                "Component "
                + std::string(typeid(*component.lock()).name())
                + " does not have a Port named '"
                + portName
                + "'."
            );
        }
        return std::make_pair(componentID, portName);
    }

};


}
}



