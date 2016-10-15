#pragma once


#include <dodo2/graph/CoordinateGraph.hpp>
#include <boost/graph/grid_graph.hpp>
#include "SimulationDomain.hpp"
#include "SimulationDomainMapInterface.hpp"
#include "Traits/Directions.hpp"


namespace dodo
{
namespace model
{
namespace data
{

    class WrappedGrid2D;
namespace traits{
    template<>
    struct Directions<WrappedGrid2D>
    {
        enum class Values
        {
            NORTH,
            EAST,
            SOUTH,
            WEST,
            SELF
        };
    };
}

    class WrappedGrid2D :
        public SimulationDomain,
        public SimulationDomainMapInterface< WrappedGrid2D >
    {
    private:
        using Directions = traits::Directions<WrappedGrid2D>::Values;
        std::map<
            graph::CoordinateGraph::EdgeID,
            Directions
        > edge2Direction;
        boost::associative_property_map< decltype( edge2Direction ) > directionMap;

        auto
        enum2String( Directions d )
        const
        -> std::string
        {
            switch(d)
            {
                case Directions::NORTH:
                    return "NORTH";
                case Directions::EAST:
                    return "EAST";
                case Directions::SOUTH:
                    return "SOUTH";
                case Directions::WEST:
                    return "WEST";
                case Directions::SELF:
                    return "SELF";
                //default:
                //    throw std::runtime_error("unknown direction!");
            }
        }

        WrappedGrid2D() :
            edge2Direction{ },
            directionMap { edge2Direction },
            vertex2Coordinate{ },
            coordinateMap{ vertex2Coordinate }
        {}

    public:

        using PosID = graph::CoordinateGraph::VertexID;
        std::map<
            graph::CoordinateGraph::VertexID,
            std::array<std::size_t, 2>
        > vertex2Coordinate;
        boost::associative_property_map< decltype(vertex2Coordinate) > coordinateMap;

        virtual
        auto
        getNeighbor(
            PosID const pos,
            Directions const neighbor
        ) const
        -> PosID override
        {
            auto edges = g.getOutEdges( pos );
            for( auto e : boost::make_iterator_range( edges ) )
            {
                if( edge2Direction.at(e) == neighbor )
                {
                    return e.m_target;
                }
            }
            throw std::runtime_error(
                "Pos " +
                std::to_string( pos ) +
                " does not have a neighbor at edge " +
                enum2String( neighbor )
            );

        }

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
//            result.coordinateMap = get(boost::vertex_bundle, grid);

//            something like this might be safer (but slower):
//            for(GridTraits::vertex_descriptor v : boost::make_iterator_range(vertices(grid)))
//            {
//                GridTraits::vertices_size_type vid = get(boost::vertex_index, grid, v);
//                put( result.coordinateMap, vid, {{ v[0], v[1] }} );
//            }

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
                    assert(vid2 < n);

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
                    graph::CoordinateGraph::EdgeID edge = simGrid.addEdge(vid, vid2);
                    put(result.directionMap, edge, var);
                }
                graph::CoordinateGraph::EdgeID selfEdge = simGrid.addEdge(vid, vid);
                put(result.directionMap, selfEdge, Directions::SELF);
            }
            result.g = simGrid;
            return result;
        }


    };

} /* data */
} /* model */
} /* dodo */
