#pragma once


#include "../graph/SimpleBGL.hpp"
#include "Component.hpp"

namespace dodo
{
namespace components
{

struct DependencyEdge
{
    Component::PortKey from;
    Component::PortKey to;
};

class DependencyBGL :
    public graph::SimpleBGL<
        boost::no_property,
        DependencyEdge,
        boost::vecS,
        boost::listS
    >{};


}
}

