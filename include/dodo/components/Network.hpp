#pragma once


#include <boost/bimap.hpp>
#include "Connection.hpp"
#include "DependencyBGL.hpp"
#include "Component.hpp"
#include "OutPort.hpp"
#include "InPort.hpp"
#include "ComponentHandle.hpp"


namespace dodo
{
namespace components
{


class Network
{
    /**
     * Holds all the Graphs that describe dependencies etc.
     *
     * Is used to connect existing components (do they need to be wrapped once more?)
     *
     * Can also be used to query information and to insert constructs like farms?
     *
     */
    using DependencyGraph = std::shared_ptr<DependencyBGL>;
    using Components = std::set<std::shared_ptr<Component>>;

    // owner_less is needed to compare the weak_pointers correctly
    using CPtr = std::weak_ptr<Component>;
    using DependencyMap = boost::bimap<
        boost::bimaps::set_of<CPtr, std::owner_less<CPtr>>,
        DependencyBGL::VertexID
    >;

    DependencyGraph dependencies;
    DependencyMap dependencyMap;
    Components components;
public:

    // Create a new instance of a component
    template<class T_Component>
    auto addComponent() -> ComponentHandle;

    void addDependency(
        std::pair<std::weak_ptr<Component>, Component::PortKey>,
        std::pair<std::weak_ptr<Component>, Component::PortKey>
    );
    Network()
    {
        dependencies = std::make_shared<DependencyBGL>();
    }

    void enable()
    {
        for (auto component : components)
        {
            component->enable();
        }
    }

};


template<class T_Component>
auto
Network::addComponent()
-> ComponentHandle
{
    std::shared_ptr<Component> comp = std::make_shared<T_Component>();
    components.insert(comp);
    DependencyBGL::VertexID depVertex = dependencies->addVertex();
    dependencyMap.left.insert({std::weak_ptr<Component>(comp), depVertex});
    return {comp};
}


    void
    Network::addDependency(
        std::pair<std::weak_ptr<Component>, Component::PortKey> portA,
        std::pair<std::weak_ptr<Component>, Component::PortKey> portB
    ){
        using VertexID = DependencyBGL::VertexID;
        auto compA = portA.first;
        auto compB = portB.first;
        VertexID dvA = dependencyMap.left.at(compA);
        VertexID dvB = dependencyMap.left.at(compB);
        dependencies->addEdge(
            dvA,
            dvB,
            {
                portA.second,
                portB.second
            }
        );
    }


}
}


