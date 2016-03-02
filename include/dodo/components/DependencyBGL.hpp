#pragma once

#include <boost/graph/graphml.hpp>

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

struct DependencyVertex
{
    types::ResourceID id;
};

class DependencyBGL :
    public graph::SimpleBGL<
        DependencyVertex,
        DependencyEdge,
        boost::vecS,
        boost::listS
    >{};


std::ostream& operator<<(std::ostream& os, const DependencyBGL& graph)
{
    using IndexMap = std::map<DependencyBGL::VertexID , std::size_t>;
    IndexMap im;
    boost::associative_property_map<IndexMap> propMapIndex(im);
    auto allV = graph.getVertices();
    int index=0;

    // ensure monotone values for the node-IDs (necessary when using listS)
    for(auto i=allV.first ; i!=allV.second ; ++i)
    {
        propMapIndex[*i] = index++;
    }

    auto& g = *(graph.graph);
    boost::dynamic_properties dp;

    // extract all interesting properties
    dp.property("id", boost::get(&DependencyVertex::id, g));
    dp.property("from_Port", boost::get(&DependencyEdge::from, g));
    dp.property("to_Port", boost::get(&DependencyEdge::to, g));

    boost::write_graphml(os, g, propMapIndex, dp);
    return os;
}


}
}

