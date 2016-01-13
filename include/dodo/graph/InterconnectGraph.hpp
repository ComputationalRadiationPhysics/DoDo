#pragma once

#include <memory>

#include <boost/hana.hpp>

#include <dout/dout.hpp>

#include <dodo/graph/Property.hpp>
#include <dodo/graph/AttributeMapStore.hpp>
#include <dodo/graph/BGL.hpp>
#include <dodo/graph/HWNode.hpp>
#include <dodo/graph/InterconnectEdge.hpp>


namespace dodo
{
namespace graph
{

namespace hana = boost::hana;


template<typename T_InterconnectProperties>
class InterconnectGraph :
    BGL<SimpleProperty, T_InterconnectProperties>
{
public:
    using Properties = T_InterconnectProperties;
    using Graph =  BGL<SimpleProperty, T_InterconnectProperties>;
    using VertexID = typename Graph::VertexID;
    using EdgeID = typename Graph::EdgeID;

    using StableVertexID = typename Graph::VertexPropertyBundle::first_type;

    using Mapping = std::map<utility::TreeID, VertexID, utility::TreeIDLess>;
    using StableMapping = std::map<utility::TreeID, StableVertexID, utility::TreeIDLess>;

    Mapping mapping;
    StableMapping stableMapping;

    VertexID add(const utility::TreeID& tid)
    {
        auto id = this->addVertex();
        mapping.insert({tid, id});
        stableMapping.insert({tid, this->getVertexProperty(id).first});
        return id;
    }

    InterconnectEdge<InterconnectGraph> connect(
        const utility::TreeID& a,
        const utility::TreeID& b
    )
    {
        auto id = this->addEdge(mapping[a], mapping[b]);
        return InterconnectEdge<InterconnectGraph>(id, this);
    }


};


} /* graph */
} /* dodo */
