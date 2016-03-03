#include <iostream>

#include <dodo.hpp>

int main( )
{
    using Speed = dodo::hardware::attributes::Speed;
    using Energy = dodo::hardware::attributes::EnergyLevel;
    using SEGraph = dodo::graph::AttributeGraph< Speed, Energy >;
    SEGraph graph;

    // create the elements that will be used to build the graph
    //
    auto laserCPUElement = graph.createComputeElement( );
    laserCPUElement.setEntry( Speed{ 3000. } );
    laserCPUElement.setEntry( Energy{ 80 } );

    auto laserNodeElement = graph.createStructuralElement( );
    laserNodeElement.setEntry( Energy{ 200 } );

    // construct the actual graph
    auto hypnos = graph.add();
    auto laserQueue = graph.add();

    hypnos.consistsOf(laserQueue);

    // Assume 2 Laser Nodes in the queue
    for( int i = 0; i < 2 ; ++i )
    {
        auto laserNode = graph.add(laserNodeElement);
        laserQueue.consistsOf(laserNode);

        // 8 Cores per Node
        for( int j = 0; j < 8 ; ++j )
        {
            auto laserCPU = graph.add(laserCPUElement);
            laserNode.consistsOf(laserCPU);
        }
    }

    auto iterators = graph.getVertices( );
    for( auto i = iterators.first ; i != iterators.second ; ++i )
    {
        std::cout << *i << "  " << graph.getVertexProperty( *i ).first << std::endl;
    }

    using SGraph = dodo::graph::AttributeGraph< Speed >;
    SGraph g2 = dodo::graph::transformInto< SGraph >( graph );

    auto iterators2 = g2.getVertices( );
    for( auto i = iterators2.first ; i != iterators2.second ; ++i )
    {
        std::cout << *i << "  " << g2.getVertexProperty( *i ).first << std::endl;
    }

    return 0;

}
