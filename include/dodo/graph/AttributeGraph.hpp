#pragma once

#include <memory>

#include <dodo/graph/Property.hpp>
#include <dodo/graph/AttributeMapStore.hpp>
#include <dodo/graph/BGL.hpp>

namespace dodo{
namespace graph{


template<typename... AttributeTypes>
class AttributeGraph : public BGL<Property<AttributeTypes...>, Property<AttributeTypes...>>
{
public:
    using AttributesTuple = std::tuple<AttributeTypes...>;
    using StoreType = AttributeMapStore<AttributeTypes...>;
    std::shared_ptr<StoreType> attributeMapStore;

    using PropertyType = Property<AttributeTypes...>;
    using Graph = typename BGL<PropertyType, PropertyType>::BGLGraph;
    using VertexID = typename Graph::vertex_descriptor;
    using index_map_t = typename boost::property_map<Graph, boost::vertex_index_t>::type;
    using IsoMap = boost::iterator_property_map<typename std::vector<VertexID>::iterator, index_map_t, VertexID, VertexID&>;
    IsoMap mappingToStructureGraph;

    AttributeGraph()
      :  attributeMapStore(StoreType::createInstance())
    {}

    AttributeGraph(std::shared_ptr<StoreType> existingStore)
      : attributeMapStore(existingStore)
    {}

    template<typename... T>
    PropertyType createProperty(T... attributes)
    {
        return PropertyType(attributeMapStore, attributes...);
    }

};

template<typename Graph>
Graph cloneView(const Graph& origin)
{
    // copy the pointer to the store
    Graph copy(origin.attributeMapStore);

    // create a map to store the mapping (initializes with old values)
    std::vector<typename Graph::VertexID> orig2copy_data(origin.numVertices());
    typename Graph::IsoMap m(
            make_iterator_property_map(orig2copy_data.begin(), get(boost::vertex_index, *(origin.graph)  )));

    // copy the graph and attach the mapping to it
    boost::copy_graph(*(origin.graph), *(copy.graph), boost::orig_to_copy(m));
    copy.mappingToStructureGraph = m;
    return copy;
}

template<typename AttributeGraph1, typename AttributeGraph2>
struct transform_vertex_copier
{
    using Graph1 = typename AttributeGraph1::BGLGraph;
    using Graph2 = typename AttributeGraph2::BGLGraph;

    transform_vertex_copier(const Graph1& g1, Graph2& g2, const AttributeGraph2& ag2)
      : vertex_all_map1(boost::get(boost::vertex_bundle, g1))
      , vertex_all_map2(boost::get(boost::vertex_bundle, g2))
      , ams2_ptr(ag2.attributeMapStore)
    {}
    template <typename Vertex1, typename Vertex2>
    void operator()(const Vertex1& v1, Vertex2& v2) const {
        //should be a pair <unsigned, Property<Attributes...>>
        typename AttributeGraph1::VertexPropertyBundle v1prop = vertex_all_map1[v1];

        typename AttributeGraph2::VertexPropertyBundle v2prop;
        v2prop.first = v1prop.first;
        v2prop.second = typename AttributeGraph2::PropertyType(ams2_ptr);
        v1prop.second.remapHandles(v2prop.second);

        put(vertex_all_map2, v2, v2prop);
    }

            typename boost::property_map<Graph1, boost::vertex_bundle_t>::const_type vertex_all_map1;
    mutable typename boost::property_map<Graph2, boost::vertex_bundle_t>::type       vertex_all_map2;
    std::shared_ptr<typename AttributeGraph2::StoreType> ams2_ptr;
};

template<typename DestinationGraph, typename AttributeGraph1, typename AttributeGraph2>
struct merge_vertex_copier
{
    using Destination = typename DestinationGraph::BGLGraph;
    using Graph1 = typename AttributeGraph1::BGLGraph;
    using Graph2 = typename AttributeGraph2::BGLGraph;

    merge_vertex_copier(Destination& d, const Graph1& g1, const Graph2& g2, const AttributeGraph2& ag2)
      : vertex_all_map0(boost::get(boost::vertex_bundle, d))
      , vertex_all_map1(boost::get(boost::vertex_bundle, g1))
      , vertex_all_map2(boost::get(boost::vertex_bundle, g2))
      , ams2_ptr(ag2.attributeMapStore)
    {}
    template <typename Vertex1, typename Vertex2>
    void operator()(const Vertex1& v1, Vertex2& v2) const {
        //should be a pair <unsigned, Property<Attributes...>>
        typename AttributeGraph1::VertexPropertyBundle v1prop = vertex_all_map1[v1];

        typename AttributeGraph2::VertexPropertyBundle v2prop;
        v2prop.first = v1prop.first;
        v2prop.second = typename AttributeGraph2::PropertyType(ams2_ptr);
        v1prop.second.remapHandles(v2prop.second);

        put(vertex_all_map2, v2, v2prop);
    }

    mutable typename boost::property_map<Destination, boost::vertex_bundle_t>::type  vertex_all_map0;
            typename boost::property_map<Graph1, boost::vertex_bundle_t>::const_type vertex_all_map1;
            typename boost::property_map<Graph2, boost::vertex_bundle_t>::const_type vertex_all_map2;

    std::shared_ptr<typename AttributeGraph2::StoreType> ams2_ptr;
};

template<typename AttributeGraph1, typename AttributeGraph2>
struct delete_vertex_copier
{
    using Graph1 = typename AttributeGraph1::BGLGraph;
    using Graph2 = typename AttributeGraph2::BGLGraph;

    delete_vertex_copier(const Graph1& g1, Graph2& g2, const AttributeGraph2& ag2)
      : vertex_all_map1(boost::get(boost::vertex_bundle, g1))
      , vertex_all_map2(boost::get(boost::vertex_bundle, g2))
      , ams2_ptr(ag2.attributeMapStore)
    {}
    template <typename Vertex1, typename Vertex2>
    void operator()(const Vertex1& v1, Vertex2& v2) const {
        //should be a pair <unsigned, Property<Attributes...>>
        typename AttributeGraph1::VertexPropertyBundle v1prop = vertex_all_map1[v1];

        typename AttributeGraph2::VertexPropertyBundle v2prop;
        v2prop.first = v1prop.first;
        v2prop.second = typename AttributeGraph2::PropertyType(ams2_ptr);

        put(vertex_all_map2, v2, v2prop);
    }

            typename boost::property_map<Graph1, boost::vertex_bundle_t>::const_type vertex_all_map1;
    mutable typename boost::property_map<Graph2, boost::vertex_bundle_t>::type       vertex_all_map2;
    std::shared_ptr<typename AttributeGraph2::StoreType> ams2_ptr;

};

template<typename AttributeGraph1, typename AttributeGraph2>
struct transform_edge_copier
{
    using Graph1 = typename AttributeGraph1::BGLGraph;
    using Graph2 = typename AttributeGraph2::BGLGraph;

    transform_edge_copier(const Graph1& g1, Graph2& g2, const AttributeGraph2& ag2)
      : edge_all_map1(boost::get(boost::edge_bundle, g1))
      , edge_all_map2(boost::get(boost::edge_bundle, g2))
      , ams2_ptr(ag2.attributeMapStore)
    {}
    template <typename Edge1, typename Edge2>
    void operator()(const Edge1& e1, Edge2& e2) const {
        //should be a pair <unsigned, Property<Attributes...>>
        typename AttributeGraph1::EdgePropertyBundle e1prop = edge_all_map1[e1];
        typename AttributeGraph2::EdgePropertyBundle e2prop;

        e2prop.first = e1prop.first;
        e2prop.second = typename AttributeGraph2::PropertyType(ams2_ptr);
        e1prop.second.remapHandles(e2prop.second);

        put(edge_all_map2, e2, e2prop);
    }

            typename boost::property_map<Graph1, boost::edge_bundle_t>::const_type edge_all_map1;
    mutable typename boost::property_map<Graph2, boost::edge_bundle_t>::type       edge_all_map2;
    std::shared_ptr<typename AttributeGraph2::StoreType> ams2_ptr;

};

template<typename AttributeGraph1, typename AttributeGraph2>
struct delete_edge_copier
{
    using Graph1 = typename AttributeGraph1::BGLGraph;
    using Graph2 = typename AttributeGraph2::BGLGraph;

    delete_edge_copier(const Graph1& g1, Graph2& g2, const AttributeGraph2& ag2)
      : edge_all_map1(boost::get(boost::edge_bundle, g1))
      , edge_all_map2(boost::get(boost::edge_bundle, g2))
      , ams2_ptr(ag2.attributeMapStore)
    {}
    template <typename Edge1, typename Edge2>
    void operator()(const Edge1& e1, Edge2& e2) const {
        //should be a pair <unsigned, Property<Attributes...>>
        typename AttributeGraph1::EdgePropertyBundle e1prop = edge_all_map1[e1];
        typename AttributeGraph2::EdgePropertyBundle e2prop;

        e2prop.first = e1prop.first;
        e2prop.second = typename AttributeGraph2::PropertyType(ams2_ptr);
        put(edge_all_map2, e2, e2prop);
    }

            typename boost::property_map<Graph1, boost::edge_bundle_t>::const_type edge_all_map1;
    mutable typename boost::property_map<Graph2, boost::edge_bundle_t>::type       edge_all_map2;
    std::shared_ptr<typename AttributeGraph2::StoreType> ams2_ptr;
};

template<typename GraphDestination, typename GraphOrigin>
GraphDestination transformInto(const GraphOrigin& origin)
{
    GraphDestination copy;

    transform_vertex_copier <GraphOrigin, GraphDestination> vc(*(origin.graph), *(copy.graph), copy);
    transform_edge_copier   <GraphOrigin, GraphDestination> ec(*(origin.graph), *(copy.graph), copy);
    boost::copy_graph(*(origin.graph), *(copy.graph), boost::vertex_copy(vc).edge_copy(ec));

    // create a map to store the mapping (initializes with old values)
    /* std::vector<typename GraphDestination::VertexID> orig2copy_data(origin.numVertices()); */
    /* typename GraphOrigin::IsoMap m( */
    /*         make_iterator_property_map(orig2copy_data.begin(), get(boost::vertex_index, *(origin.graph)  ))); */
    /* boost::copy_graph(*(origin.graph), *(copy.graph), boost::vertex_copy(vc).edge_copy(ec).orig_to_copy(m)); */
    /* copy.mappingToStructureGraph = m; */
    return copy;
}

template<typename GraphDestination, typename GraphOrigin>
GraphDestination transformIntoEdgeOnly(const GraphOrigin& origin)
{
    GraphDestination copy;

    delete_vertex_copier  <GraphOrigin, GraphDestination> vc(*(origin.graph), *(copy.graph), copy);
    transform_edge_copier <GraphOrigin, GraphDestination> ec(*(origin.graph), *(copy.graph), copy);
    boost::copy_graph(*(origin.graph), *(copy.graph), boost::vertex_copy(vc).edge_copy(ec));
    return copy;
}

template<typename GraphDestination, typename GraphOrigin>
GraphDestination transformIntoVertexOnly(const GraphOrigin& origin)
{
    GraphDestination copy;

    transform_vertex_copier <GraphOrigin, GraphDestination> vc(*(origin.graph), *(copy.graph), copy);
    delete_edge_copier      <GraphOrigin, GraphDestination> ec(*(origin.graph), *(copy.graph), copy);
    boost::copy_graph(*(origin.graph), *(copy.graph), boost::vertex_copy(vc).edge_copy(ec));
    return copy;
}

/* // graphs must be structurally the same! */
/* template<typename GraphDestination, typename Graph1, typename Graph2> */
/* GraphDestination merge(const Graph1& graph1, const Graph2& graph2) */
/* { */
/*     GraphDestination dest; */

/*     merge_vertex_copier <Graph1, Graph2, GraphDestination> vc(*(graph1.graph), *(graph2.graph), *(dest.graph)); */
/*     //delete_edge_copier      <GraphOrigin, GraphDestination> ec(*(origin.graph), *(copy.graph)); */
/*     //boost::copy_graph(*(origin.graph), *(copy.graph), boost::vertex_copy(vc).edge_copy(ec)); */
/*     boost::copy_graph(*(origin.graph), *(copy.graph), boost::vertex_copy(vc)); */
/*     return copy; */
/* } */


} /* graph */
} /* dodo */
