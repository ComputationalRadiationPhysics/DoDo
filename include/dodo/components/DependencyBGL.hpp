#pragma once


#include "../graph/SimpleBGL.hpp"
#include "ComponentMetadataInterface.hpp"

namespace dodo
{
namespace components
{


struct DependencyEdge
{
    ComponentMetadataInterface::PortKey from;
    ComponentMetadataInterface::PortKey to;
};

class DependencyBGL :
    public graph::SimpleBGL<
        std::string,
        DependencyEdge,
        boost::vecS,
        boost::listS
    >{};


}
}

