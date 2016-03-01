#pragma once


#include "../graph/SimpleBGL.hpp"
#include "types.hpp"

namespace dodo
{
namespace components
{

struct DependencyEdge
{
    types::PortKey from;
    types::PortKey to;
};

class DependencyBGL :
    public graph::SimpleBGL<
        types::ResourceID,
        DependencyEdge,
        boost::vecS,
        boost::listS
    >{};


}
}

