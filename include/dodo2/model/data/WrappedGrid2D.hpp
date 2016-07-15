#pragma once


#include <dodo2/graph/CoordinateGraph.hpp>
#include <boost/graph/grid_graph.hpp>
#include "SimulationDomain.hpp"
#include "SimulationDomainMapInterface.hpp"


namespace dodo
{
namespace model
{
namespace data
{

    class WrappedGrid2D :
        public SimulationDomain,
        public SimulationDomainMapInterface< WrappedGrid2D >
    {
    public:
        enum class Directions
        {
            NORTH,
            EAST,
            SOUTH,
            WEST,
            _count
        };


        std::map<
            graph::CoordinateGraph::VertexID,
            std::array<std::size_t, 2>
        > vertex2Coordinate;
        boost::associative_property_map< decltype(vertex2Coordinate) > coordinateMap;

        static
        WrappedGrid2D
        generate(
            std::size_t dimx,
            std::size_t dimy
        )
        {
            constexpr int dim = 2;
            const boost::array< std::size_t, dim > lengths = {{ dimx, dimy }};
            const boost::grid_graph< dim > grid( lengths, {{true, true}} );
            using GridTraits = boost::graph_traits< boost::grid_graph< dim > >;
            auto n = num_vertices(grid);
            graph::CoordinateGraph simGrid(n);

            WrappedGrid2D result;
            // TODO: check the following:
            // attention: this might lead to a segfault, not sure what happens
            // with the map from the grid once the static function is finished
            result.coordinateMap = get(boost::vertex_bundle, grid);

//            something like this might be safer (but slower):
//            for(GridTraits::vertex_descriptor v : boost::make_iterator_range(vertices(grid)))
//            {
//                GridTraits::vertices_size_type vid = get(boost::vertex_index, grid, v);
//                put( result.coordinateMap, vid, {{ v[0], v[1] }} );

            for( decltype(n) vid=0 ; vid< n ; ++vid )
            {
                auto v = vertex( vid, grid );

                for(GridTraits::edge_descriptor e : boost::make_iterator_range(out_edges(v, grid)))
                {
                    std::size_t fromx = e.first[0];
                    std::size_t fromy = e.first[1];
                    std::size_t tox = e.second[0];
                    std::size_t toy = e.second[1];

                    GridTraits::vertices_size_type vid2 = get(boost::vertex_index, grid, e.second);
                    assert(vid2 >= 0 && vid2 < n);

                    Directions var;
                    if(fromx == (tox+1)%dimx)
                    {
                        var = Directions::EAST;
                    }
                    if((fromx+1)%dimx == tox)
                    {
                        var = Directions::WEST;
                    }
                    if((fromy+1)%dimy == toy)
                    {
                        var = Directions::SOUTH;
                    }
                    if(fromy == (toy+1)%dimy)
                    {
                        var = Directions::NORTH;
                    }
                    graph::CoordinateGraph::EdgeID e = simGrid.addEdge(vid, vid2);

                    put(result.directionMap, e, var);
                }
            }
            result.g = simGrid;
            return result;
        }


    };

} /* data */
} /* model */
} /* dodo */
