#pragma once


#include <boost/bimap.hpp>
#include "Connection.hpp"
#include "DependencyBGL.hpp"
#include "ComponentImpl.hpp"
#include "OutPort.hpp"
#include "InPort.hpp"


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
    auto addComponent() -> std::weak_ptr<T_Component>;

    void addDependency(std::weak_ptr<OutPort>, std::weak_ptr<InPort>);

};


template<class T_Component>
auto
Network::addComponent()
-> std::weak_ptr<T_Component>
{
    std::shared_ptr<Component> comp = std::make_shared<T_Component>();
    components.insert(comp);
    DependencyBGL::VertexID depVertex = dependencies->addVertex();
    dependencyMap.left.insert({std::weak_ptr<T_Component>(comp), depVertex});
    return comp;
}

void
Network::addDependency(
    std::weak_ptr<OutPort> portA,
    std::weak_ptr<InPort> portB
){
    using VertexID = DependencyBGL::VertexID;
    std::weak_ptr<Component> compA = portA.lock()->component;
    std::weak_ptr<Component> compB = portB.lock()->component;
    VertexID dvA = dependencyMap.left.at(compA);
    VertexID dvB = dependencyMap.left.at(compB);
    dependencies->addEdge(dvA, dvB, {portA, portB});
}


}
}


