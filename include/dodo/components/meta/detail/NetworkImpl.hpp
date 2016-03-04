#pragma once

//#include <map>
//#include <string>
//
//#include "dodo/components/types.hpp"
//#include "dodo/components/meta/Interface.hpp"
//#include "dodo/components/dependency/BGL.hpp"


#include <bits/shared_ptr.h>


namespace dodo
{
namespace components
{
namespace meta
{
namespace detail
{

/**
 * This class is not intended to be instantiated manually.
 * Instead, use the Network class to wrap access.
 */
class NetworkImpl :
    public std::enable_shared_from_this<NetworkImpl>
{
    /**
     * Holds all the Graphs that describe dependencies etc.
     *
     * Is used to connect existing components (do they need to be wrapped once more?)
     *
     * Can also be used to query information and to insert constructs like farms?
     *
     */

private:
    class ComponentHandle;


    using ResourceID = types::ResourceID;
    using DependencyGraph = std::shared_ptr<dependency::BGL>;
    using Components = std::map<ResourceID, std::shared_ptr<meta::Interface>>;
    using DependencyMap = std::map<
        ResourceID,
        dependency::BGL::VertexID
    >;
    DependencyGraph dependencies;
    DependencyMap dependencyMap;
    Components components;

public:
    NetworkImpl();

    template<class T_Component>
    auto addComponent();

    std::weak_ptr<meta::Interface> getComponent(ResourceID id);

    void addDependency(
        std::pair<
            ResourceID,
            types::PortKey
        > portA,
        std::pair<
            ResourceID,
            types::PortKey
        > portB
    );


    std::weak_ptr<dependency::BGL> getDependencies()
    {
        return std::weak_ptr<dependency::BGL>(dependencies);
    }


};


std::weak_ptr<Interface> NetworkImpl::getComponent(ResourceID id)
{
    return components.at(id);
}


NetworkImpl::NetworkImpl()
{
    dependencies = std::make_shared<dependency::BGL>();
}


class NetworkImpl::ComponentHandle
{
public:
    std::weak_ptr<NetworkImpl> net;
    ResourceID componentID;


    ComponentHandle(std::weak_ptr<NetworkImpl> net, const ResourceID c) :
        net(net),
        componentID(c)
    {
    }


    auto  operator[](std::string portName) const;

};


void NetworkImpl::addDependency(
    std::pair<ResourceID, types::PortKey> portA,
    std::pair<ResourceID, types::PortKey> portB
)
{
    using VertexID = dependency::BGL::VertexID;
    auto compA = portA.first;
    auto compB = portB.first;
    VertexID dvA = dependencyMap.at(compA);
    VertexID dvB = dependencyMap.at(compB);
    dependencies->addEdge(
        dvA,
        dvB,
        {
            portA.second,
            portB.second
        }
    );
}


template<class T_Component> auto NetworkImpl::addComponent()
{
    std::shared_ptr<meta::Interface> comp = std::make_shared<T_Component>();
    std::string myKey = std::to_string(reinterpret_cast<size_t>(comp.get()));
    components[myKey] = comp;
    dependency::BGL::VertexID depVertex = dependencies->addVertex({myKey});
    dependencyMap[myKey] = depVertex;
    return ComponentHandle(shared_from_this(), myKey);
}


auto  NetworkImpl::ComponentHandle::operator[](std::string portName) const
{
    auto component = net.lock()->getComponent(componentID);
    if (!component.lock()->hasPort(portName))
    {
        throw std::runtime_error(
            std::string("Component ")
            + typeid(*component.lock()).name()
            + " does not have a Port named '"
            + portName
            + "'."
        );
    }
    return std::make_pair(componentID, portName);
}


}
}
}
}

