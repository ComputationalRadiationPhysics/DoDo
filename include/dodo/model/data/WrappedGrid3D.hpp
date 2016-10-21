#pragma once


#include <dodo/graph/CoordinateGraph.hpp>
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

    class WrappedGrid3D;
namespace traits{
    template<>
    struct Directions<WrappedGrid3D>
    {
        enum class Values
        {
            NORTH,
            EAST,
            SOUTH,
            WEST,
            UP,
            DOWN
        };
    };
}

    class WrappedGrid3D :
        public SimulationDomain,
        public SimulationDomainMapInterface< WrappedGrid3D >
    {
    public:

    private:
        using Directions = traits::Directions<WrappedGrid3D>::Values;
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
                case Directions::UP:
                    return "UP";
                case Directions::DOWN:
                    return "DOWN";
                //default:
                //    throw std::runtime_error("unknown direction!");
            }
            return "";
        }

        WrappedGrid3D() :
            edge2Direction{},
            directionMap{edge2Direction},
            vertex2Coordinate{},
            coordinateMap{vertex2Coordinate}
        {}

    public:

        using PosID = graph::CoordinateGraph::VertexID;
        std::map<
            graph::CoordinateGraph::VertexID,
            std::array<std::size_t, 3>
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
        WrappedGrid3D
        generate(
            std::size_t dimx,
            std::size_t dimy,
            std::size_t dimz
        )
        {
            constexpr int dim = 3;
            const boost::array< std::size_t, dim > lengths = {{ dimx, dimy, dimz }};
            const boost::grid_graph< dim > grid( lengths, {{true, true, true}} );
            using GridTraits = boost::graph_traits< boost::grid_graph< dim > >;
            auto n = num_vertices(grid);
            graph::CoordinateGraph simGrid(n);

            WrappedGrid3D result;
            // TODO: check the following:
            // attention: this might lead to a segfault, not sure what happens
            // with the map from the grid once the static function is finished
//            result.coordinateMap = boost::get(boost::vertex_bundle_t, grid);

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
                    std::size_t fromz = e.first[2];
                    std::size_t tox = e.second[0];
                    std::size_t toy = e.second[1];
                    std::size_t toz = e.second[2];

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
                    if(fromz == (toz+1)%dimz)
                    {
                        var = Directions::DOWN;
                    }
                    if((fromz+1)%dimz == toz)
                    {
                        var = Directions::UP;
                    }
                    graph::CoordinateGraph::EdgeID edge = simGrid.addEdge(vid, vid2);

//                    result.edge2Direction.insert(std::make_pair(edge,var));
                    put(result.directionMap, edge, var);
                }
            }
            result.g = simGrid;
            return result;
        }


    };

} /* data */
} /* model */
} /* dodo */
