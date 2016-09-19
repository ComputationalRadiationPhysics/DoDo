#include <iostream>
#include <vector>
#include <chrono>
#include <sys/resource.h>

#include <boost/config.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/program_options.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>


#include <stdlib.h>



int main()
{

    using Graph =  boost::adjacency_list<
        boost::vecS,
        boost::vecS,
        boost::bidirectionalS,
        boost::property<boost::vertex_index_t, size_t, std::string>
    >;

    Graph g;

    auto v1 = add_vertex(g);
    auto v2 = add_vertex(g);
    auto v3 = add_vertex(g);
    auto v4 = add_vertex(g);

    // a line from v1 -> v2 -> v3
    add_edge(v1, v2, g);
    add_edge(v2, v1, g);
    add_edge(v2, v3, g);
    add_edge(v3, v2, g);
    // v4 is not connected

    std::vector< float > distanceMem( boost::num_vertices( g ) );
    auto distanceMap = boost::make_iterator_property_map(
        distanceMem.begin(),
        boost::get(boost::vertex_index, g)
    );

    std::map< Graph::edge_descriptor, float > weightMem;
    for(auto e : boost::make_iterator_range( edges( g ) ) )
    {
        weightMem.insert( std::make_pair(e, 42.5 ) );
    }
    boost::associative_property_map<decltype(weightMem)> weightMap(weightMem);

    boost::dijkstra_shortest_paths(
        g,
        v1,
        boost::distance_map( distanceMap ).weight_map( weightMap )
    );

    // TODO: the result of the djikstra algorithm may be cached
    std::cout << distanceMap[v1] << std::endl;
    std::cout << distanceMap[v2] << std::endl;
    std::cout << distanceMap[v3] << std::endl;
    std::cout << distanceMap[v4] << std::endl;

    return 0;
}


