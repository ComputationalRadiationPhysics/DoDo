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


class ResourceGraph :
    public graph::SimpleBGL<
        Vertex,
        Vertex,
        boost::vecS,
        boost::listS
    >
{
};


}
}
}

