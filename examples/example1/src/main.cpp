#include <iostream>
#include <memory> /* shared_ptr */

#include <boost/graph/adjacency_list.hpp>

#include <chunky.hpp>



int main()
{
    using namespace chunky;


    using EnergyLevel = chunky::physical::attributes::EnergyLevel;
    using AMS = chunky::graph::AttributeMapStore<EnergyLevel>;
    using ChunkyGraphProperty = typename AMS::ChunkyGraphProperty;
    using ComputeGraph = chunky::graph::BGL<ChunkyGraphProperty>;

    ComputeGraph graph;
    std::shared_ptr<AMS> ams = AMS::createInstance();

    ChunkyGraphProperty node(ams);
    node.addEntry(EnergyLevel{3});

    graph.addVertex(node.clone());


    return EXIT_SUCCESS;

}
