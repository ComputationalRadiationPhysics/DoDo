#pragma once


#include "DependencyBGL.hpp"
#include "ComponentHandle.hpp"


namespace dodo
{
namespace components
{


class MetadataNetwork
{
    /**
     * Holds all the Graphs that describe dependencies etc.
     *
     * Is used to connect existing components (do they need to be wrapped once more?)
     *
     * Can also be used to query information and to insert constructs like farms?
     *
     */
public:
    using ResourceID = std::string;

private:
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
    MetadataNetwork()
    {
        dependencies = std::make_shared<DependencyBGL>();
    }

    // Create a new instance of a component
    template<class T_Component>
    auto addComponent() -> ComponentHandle;

    void addDependency(
        std::pair<std::weak_ptr<ComponentMetadataInterface>, ComponentMetadataInterface::PortKey>,
        std::pair<std::weak_ptr<ComponentMetadataInterface>, ComponentMetadataInterface::PortKey>
    );

//    void enable()
//    {
//        // Start the execution of the components
//        for (auto component : components)
//        {
//            component->enable();
//        }
//    }

};


template<class T_Component>
auto
MetadataNetwork::addComponent()
-> ComponentHandle
{
    std::shared_ptr<ComponentMetadataInterface> comp = std::make_shared<T_Component>();
    std::string myKey= std::to_string(reinterpret_cast<size_t>(comp.get()));
    components[myKey] = comp;
    DependencyBGL::VertexID depVertex = dependencies->addVertex();
    dependencyMap[myKey] = depVertex;
    // make shared from net
    return {net, comp};
}


    void
    MetadataNetwork::addDependency(
        std::pair<std::weak_ptr<ComponentMetadataInterface>, ComponentMetadataInterface::PortKey> portA,
        std::pair<std::weak_ptr<ComponentMetadataInterface>, ComponentMetadataInterface::PortKey> portB
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


