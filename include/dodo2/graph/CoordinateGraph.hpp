#pragma once

#include "dodo2/graph/SimpleBGL.hpp"
#include "dodo2/utility/tree_id.hpp"

namespace dodo
{
namespace graph
{

    class CoordinateGraph :
        public graph::SimpleBGL< >
    {
    public:
        using SBGL = graph::SimpleBGL<
            boost::no_property,
            boost::property<
                boost::edge_index_t,
                std::size_t
            >
        >;
        using VertexID = SBGL::VertexID;

    };


} /* graph */
} /* dodo */
