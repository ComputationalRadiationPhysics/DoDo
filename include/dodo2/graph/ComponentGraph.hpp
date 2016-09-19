#pragma once


#include <boost/graph/properties.hpp>
#include <boost/pending/property.hpp>

#include "dodo2/graph/SimpleBGL.hpp"
#include "dodo2/utility/tree_id.hpp"

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

        ComponentGraph() = default;

        ComponentGraph( std::size_t nVertices ) :
            SBGL( nVertices )
        { }

    };


} /* graph */
} /* dodo */
