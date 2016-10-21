#pragma once

#include "dodo/graph/SimpleBGL.hpp"
#include "dodo/utility/tree_id.hpp"

namespace dodo
{
namespace graph
{

    class DataGraph :
        public graph::SimpleBGL<
            boost::property<
                boost::vertex_index_t,
                size_t
            >,
            boost::no_property,
            boost::vecS,
            boost::listS,
            boost::directedS
        >
    {
    public:
        using SBGL = graph::SimpleBGL<
            boost::property<
                boost::vertex_index_t,
                size_t
            >,
            boost::no_property,
            boost::vecS,
            boost::listS,
            boost::directedS
        >;
        using VertexID = SBGL::VertexID;
    };


} /* graph */
} /* dodo */
