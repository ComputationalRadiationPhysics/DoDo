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

template<typename... AttributeTypes>
class InterconnectGraph :
    BGL<SimpleProperty, Property<AttributeTypes...>>
{
public:
    using AttributesTuple = std::tuple<AttributeTypes...>;
    static constexpr auto AttributesHanaSet = hana::to_set(hana::tuple_t<AttributeTypes...>);

    using StoreType = AttributeMapStore<AttributeTypes...>;
    std::shared_ptr<StoreType> attributeMapStore;

    using PropertyType = Property<AttributeTypes...>;
    using Graph = typename BGL<PropertyType, PropertyType>::BGLGraph;
    using VertexID = typename Graph::vertex_descriptor;
    using EdgeID = typename Graph::edge_descriptor;

    VertexID root;

    InterconnectGraph() :
        attributeMapStore(std::make_shared<StoreType>()),
        root(add())
    {}

    InterconnectGraph(std::shared_ptr<StoreType> existingStore) :
        attributeMapStore(existingStore),
        root(add())
    {}

public:

    VertexID add()
    {
        return this->addVertex();
    }

    InterconnectEdge<InterconnectGraph> connect(VertexID a, VertexID b)
    {
        auto id = this->addEdge(a, b);
        return InterconnectEdge<InterconnectGraph>(id, this);
    }




};
} /* graph */
} /* dodo */
