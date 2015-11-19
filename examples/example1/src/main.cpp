#include <iostream>

#include <chunky.hpp>

int main()
{
    using Speed                  = chunky::physical::attributes::Speed;
    using Energy                 = chunky::physical::attributes::EnergyLevel;
    using SEGraph                = chunky::graph::AttributeGraph<Speed, Energy>;
    SEGraph graph;

    // create the elements that will be used to build the graph
    //
    auto laserCPU = graph.createProperty();
    laserCPU.setEntry(Speed{3000.});
    laserCPU.setEntry(Energy{80});

    auto laserNode = graph.createProperty();
    laserNode.setEntry(Energy{200});


    // construct the actual graph
    //
    auto hypnosVID = graph.addVertex();
    auto laserQueueVID = graph.addVertex();

    graph.addEdge(hypnosVID, laserQueueVID);

    // Assume 2 Laser Nodes in the queue
    for(int i=0; i<2 ; ++i)
    {
        auto laserNodeVID = graph.addVertex(laserNode.clone());
        graph.addEdge(laserQueueVID, laserNodeVID);

        // 8 Cores per Node
        for(int j=0; j<8 ; ++j)
        {
            auto laserCPUVID = graph.addVertex(laserCPU.clone());
            graph.addEdge(laserNodeVID, laserCPUVID);
        }
    }

    using SGraph = chunky::graph::AttributeGraph<Speed>;
    SGraph speedGraph = chunky::graph::transformInto<SGraph>(graph);

    return 0;

}
