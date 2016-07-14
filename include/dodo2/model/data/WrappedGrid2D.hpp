#pragma once


#include <dodo2/graph/CoordinateGraph.hpp>
#include <boost/graph/grid_graph.hpp>
#include "SimulationDomain.hpp"


namespace dodo
{
namespace model
{
namespace data
{

    class WrappedGrid2D :
        public SimulationDomain<WrappedGrid2D>
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
            for(GridTraits::vertex_descriptor v : boost::make_iterator_range(vertices(grid)))
            {
                GridTraits::vertices_size_type vid = get(boost::vertex_index, grid, v);
                assert(vid >= 0 && vid < n);
                for(GridTraits::edge_descriptor e : boost::make_iterator_range(out_edges(v, grid)))
                {
                    GridTraits::vertex_descriptor fromx = e.first[0];
                    GridTraits::vertex_descriptor fromy = e.first[1];
                    GridTraits::vertex_descriptor tox = e.second[0];
                    GridTraits::vertex_descriptor toy = e.second[1];

                    GridTraits::vertices_size_type vid2 = get(boost::vertex_index, grid, e.second);

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

        }

        result.g = simGrid;
        return result;

    };

} /* data */
} /* model */
} /* dodo */
