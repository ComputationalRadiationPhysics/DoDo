#include <iostream>

#include <dodo.hpp>

int main( )
{
    using Speed = dodo::physical::attributes::Speed;
    using Energy = dodo::physical::attributes::EnergyLevel;
    using SEGraph = dodo::graph::AttributeGraph< Speed, Energy >;
    SEGraph graph;

    // create the elements that will be used to build the graph
    //
    auto laserCPU = graph.createProperty( );
    laserCPU.setEntry( Speed{ 3000. } );
    laserCPU.setEntry( Energy{ 80 } );

    auto laserNode = graph.createProperty( );
    laserNode.setEntry( Energy{ 200 } );


    // construct the actual graph
    //
    auto hypnosVID = graph.addVertex( );
    auto laserQueueVID = graph.addVertex( );

    graph.addEdge( hypnosVID, laserQueueVID );

    // Assume 2 Laser Nodes in the queue
    for( int i = 0; i < 2 ; ++i )
    {
        auto laserNodeVID = graph.addVertex( laserNode.clone( ) );
        graph.addEdge(laserQueueVID, laserNodeVID);

        // 8 Cores per Node
        for( int j = 0; j < 8 ; ++j )
        {
            auto laserCPUVID = graph.addVertex( laserCPU.clone( ) );
            graph.addEdge( laserNodeVID, laserCPUVID );
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
