#pragma once

#include <boost/bimap.hpp>
#include "Connection.hpp"
#include "DependencyBGL.hpp"
#include "ComponentImpl.hpp"


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
    using DependencyMap = boost::bimap<std::shared_ptr<Component>, DependencyBGL::VertexID>;

    DependencyGraph dependencies;
    DependencyMap dependencyMap;
    Components components;
public:

    // Create a new instance of a component
    template<class T_Component>
    auto addComponent() -> std::shared_ptr<T_Component>;

    template<
        typename T_ComponentA,
        typename T_ComponentB
    >
    void addDependency(std::weak_ptr<OutPort>, std::weak_ptr<InPort>);

    Network()
    {}
};

template<class T_Component>
auto
Network::addComponent()
-> std::shared_ptr<T_Component>
{
    std::shared_ptr<Component> comp = std::make_shared<T_Component>();
    components.insert(comp);
    DependencyBGL::VertexID depVertex = dependencies->addVertex();
//    dependencyMap.left.insert(std::make_pair(comp, depVertex));
    dependencyMap.left.insert({comp, depVertex});
    //dependencyMap.left[comp] = depVertex;
    return comp;
}


template<
    typename T_ComponentA,
    typename T_ComponentB
>
void Network::addDependency(
    std::weak_ptr<OutPort> portA,
    std::weak_ptr<InPort> portB
)
{
    using VertexID = DependencyBGL::VertexID;
    auto CompA = portA.lock()->component;
    auto CompB = portB.lock()->component;
    VertexID dvA = dependencyMap.left.at(CompA);
    VertexID dvB = dependencyMap.left.at(CompB);

    dependencies->addEdge(dvA, dvB, {portA, portB});
}


}
}


