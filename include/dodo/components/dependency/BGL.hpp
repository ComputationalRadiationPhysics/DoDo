#pragma once

#include <boost/graph/graphml.hpp>

#include "dodo/graph/SimpleBGL.hpp"
#include "dodo/components/types.hpp"
#include "Edge.hpp"
#include "Vertex.hpp"


namespace dodo
{
namespace components
{
namespace dependency
{


class BGL :
    public graph::SimpleBGL<
        Vertex,
        Edge,
        boost::vecS,
        boost::listS
    >
{
};


}
}
}

