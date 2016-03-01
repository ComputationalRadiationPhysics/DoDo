#pragma once

#include <map>
#include <string>
#include <memory>

#include "dodo/components/types.hpp"
#include "../ComponentMetadataInterface.hpp"
#include "../DependencyBGL.hpp"


namespace dodo
{
namespace components
{
namespace detail
{

/**
 * This class is not intended to be instantiated manually.
 * Instead, use the MetadataNetwork class to wrap access.
 */
class MetadataNetworkImpl :
    public std::enable_shared_from_this<MetadataNetworkImpl>
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
    using DependencyGraph = std::shared_ptr<DependencyBGL>;
    using Components = std::map<ResourceID, std::shared_ptr<ComponentMetadataInterface>>;
    using DependencyMap = std::map<
        ResourceID,
        DependencyBGL::VertexID
    >;
    DependencyGraph dependencies;
    DependencyMap dependencyMap;
    Components components;

public:
    MetadataNetworkImpl();

    template<class T_Component>
    auto addComponent();

    std::weak_ptr<ComponentMetadataInterface> getComponent( ResourceID id );

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

};


std::weak_ptr<ComponentMetadataInterface> MetadataNetworkImpl::getComponent(ResourceID id)
{
    return components.at( id );
}


MetadataNetworkImpl::MetadataNetworkImpl()
{
    dependencies = std::make_shared<DependencyBGL>();
}


class MetadataNetworkImpl::ComponentHandle
{
public:
    std::weak_ptr<MetadataNetworkImpl> net;
    ResourceID componentID;


    ComponentHandle( std::weak_ptr<MetadataNetworkImpl> net, const ResourceID c ) :
        net( net ),
        componentID( c )
    { }


    auto  operator[]( std::string portName ) const;

};


void MetadataNetworkImpl::addDependency(
    std::pair<ResourceID, types::PortKey> portA,
    std::pair<ResourceID, types::PortKey> portB
)
{
    using VertexID = DependencyBGL::VertexID;
    auto compA = portA.first;
    auto compB = portB.first;
    VertexID dvA = dependencyMap.at( compA );
    VertexID dvB = dependencyMap.at( compB );
    dependencies->addEdge(
        dvA,
        dvB,
        {
            portA.second,
            portB.second
        }
    );
}


template<class T_Component> auto MetadataNetworkImpl::addComponent()
{
    std::shared_ptr<ComponentMetadataInterface> comp = std::make_shared<T_Component>();
    std::string myKey = std::to_string( reinterpret_cast<size_t>(comp.get()));
    components[myKey] = comp;
    DependencyBGL::VertexID depVertex = dependencies->addVertex();
    dependencyMap[myKey] = depVertex;
    return ComponentHandle( shared_from_this(), myKey );
}


auto  MetadataNetworkImpl::ComponentHandle::operator[]( std::string portName ) const
{
    auto component = net.lock()->getComponent( componentID );
    if( !component.lock()->hasPort( portName ))
    {
        throw std::runtime_error(
            std::string("Component ")
            + typeid( *component.lock()).name()
            + " does not have a Port named '"
            + portName
            + "'."
        );
    }
    return std::make_pair( componentID, portName );
}


}
}
}

