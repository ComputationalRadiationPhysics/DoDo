#pragma once


#include <boost/graph/properties.hpp>
#include <boost/pending/property.hpp>

#include "dodo/graph/SimpleBGL.hpp"
#include "dodo/utility/tree_id.hpp"
#include "CoordinateGraph.hpp"


namespace dodo
{
namespace graph
{

    class ComponentGraph :
        public graph::SimpleBGL<
            boost::property<
                boost::vertex_index_t,
                std::size_t
            >
        >
    {
    public:
        using SBGL = graph::SimpleBGL<
            boost::property<
                boost::vertex_index_t,
                std::size_t
            >
        >;
        using VertexID = SBGL::VertexID;

        std::map<
            std::pair<std::string, CoordinateGraph::VertexID>,
            VertexID
        > inverseInstanceMap;
//        utility::PropertyManager::MapType < decltype( internal_inverseInstanceMap ) > inverseInstanceMap;

        ComponentGraph() = default;

        ComponentGraph( std::size_t nVertices ) :
            SBGL( nVertices ),
            inverseInstanceMap{ }
        { }

        VertexID
        addVertex(
            std::string name,
            CoordinateGraph::VertexID position
        )
        {
            VertexID v = SBGL::addVertex();
            inverseInstanceMap[std::make_pair(name, position)]  = v;
            return v;
        }

        auto
        addDependency(
            VertexID predecessor,
            VertexID successor
        )
        -> void
        {
            SBGL::addEdge(
                predecessor,
                successor
            );
        }

    };


} /* graph */
} /* dodo */
