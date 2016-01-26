// #pragma once
// 
// #include <memory>
// 
// #include <boost/hana.hpp>
// 
// #include <dout/dout.hpp>
// 
// #include <dodo/graph/Property.hpp>
// #include <dodo/graph/AttributeMapStore.hpp>
// #include <dodo/graph/BGL.hpp>
// #include <dodo/graph/HWNode.hpp>
// 
// namespace dodo
// {
// namespace graph
// {
// 
// namespace hana = boost::hana;
// 
// template<typename... AttributeTypes>
// class AttributeGraph :
//     public BGL<Property<AttributeTypes...>, Property<AttributeTypes...>>
// {
// public:
//     using AttributesTuple = std::tuple<AttributeTypes...>;
//     static constexpr auto AttributesHanaSet = hana::to_set(hana::tuple_t<AttributeTypes...>);
// 
//     using StoreType = AttributeMapStore<AttributeTypes...>;
//     std::shared_ptr<StoreType> attributeMapStore;
// 
//     using PropertyType = Property<AttributeTypes...>;
//     using Graph = typename BGL<PropertyType, PropertyType>::BGLGraph;
//     using VertexID = typename Graph::vertex_descriptor;
//     using EdgeID = typename Graph::edge_descriptor;
// 
//     // invariant: root vertex exists and has no incoming edges
//     VertexID root;
// 
//     AttributeGraph()
//       :  attributeMapStore(std::make_shared<StoreType>())
//     {}
// 
//     AttributeGraph(std::shared_ptr<StoreType> existingStore)
//       : attributeMapStore(existingStore)
//     {}
// 
// private:
//     template<typename... T>
//     PropertyType createProperty(T... attributes)
//     {
//         return PropertyType(attributeMapStore, attributes...);
//     }
// 
// protected:
//     template<typename Subgraph>
//     VertexID consistsOf(Subgraph g)
//     {
//         decltype(auto) dout = dout::Dout::getInstance();
//         dout(dout::Flags::DEBUG) << "in AttributeGraph::consistsOf" << std::endl;
// 
//         // attach the subtree
//         VertexID attachedRoot = attachSubtree(*this, g);
// 
//         // collect the tags from the subtree and add them to own root
//         auto attachedTags = this->getVertexProperty(attachedRoot).second.tagInfo;
//         this->getVertexProperty(root).second.tagInfo.add(std::move(attachedTags));
// 
//         auto connectingEdge = this->addEdge(root, attachedRoot);
//         auto edgeProperty = this->getEdgeProperty(connectingEdge).second;
//         edgeProperty.tagInfo.addNode(NodeType::STRUCTURAL);
//         this->setEdgeProperty(connectingEdge, edgeProperty);
// 
//         dout(dout::Flags::DEBUG) << "in AttributeGraph::consistsOf end" << std::endl;
//         return attachedRoot;
//     }
// 
// public:
//     template<typename... T>
//     PropertyType createStructuralElement(T... attributes)
//     {
//         auto p = createProperty(std::forward<T>(attributes)...);
//         p.tagInfo.addNode(NodeType::STRUCTURAL);
//         return p;
//     }
// 
//     std::pair<EdgeID, EdgeID> connectBidirectional(const VertexID a, const VertexID b)
//     {
//         auto edge1 = this->addEdge(a, b);
//         auto edge2 = this->addEdge(a, b);
// 
//         auto edgeProperty1 = this->getEdgeProperty(edge1).second;
//         auto edgeProperty2 = this->getEdgeProperty(edge2).second;
// 
//         edgeProperty1.tagInfo.addNode(NodeType::INTERCONNECT);
//         edgeProperty2.tagInfo.addNode(NodeType::INTERCONNECT);
//         this->setEdgeProperty(edge1, edgeProperty1);
//         this->setEdgeProperty(edge2, edgeProperty2);
// 
//         return std::make_pair(edge1, edge2);
//     }
// 
// 
//     template<typename... T>
//     PropertyType createComputeElement(T... attributes)
//     {
//         auto p = createProperty(std::forward<T>(attributes)...);
//         p.tagInfo.addNode(NodeType::COMPUTE);
//         return p;
//     }
// 
//     template<typename... T>
//     PropertyType createMemoryElement(T... attributes)
//     {
//         auto p = createProperty(std::forward<T>(attributes)...);
//         p.tagInfo.addNode(NodeType::MEMORY);
//         return p;
//     }
// 
//     template<typename... T>
//     PropertyType createInterconnectElement(T... attributes)
//     {
//         auto p = createProperty(std::forward<T>(attributes)...);
//         p.tagInfo.addNode(NodeType::INTERCONNECT);
//         return p;
//     }
// 
//     // HWNode<AttributeTypes...> add()
//     // {
//     //     auto id = this->addVertex();
//     //     return HWNode<AttributeTypes...>(id, this);
//     // }
// 
//     // HWNode<AttributeTypes...> add(PropertyType& p)
//     // {
//     //     auto id = this->addVertex(p.clone());
//     //     return HWNode<AttributeTypes...>(id, this);
//     // }
// 
// 
// };
// 
// template<typename Graph>
// Graph cloneView(const Graph& origin)
// {
//     // copy the pointer to the store
//     Graph copy(origin.attributeMapStore);
// 
//     // create a map to store the mapping (initializes with old values)
//     std::vector<typename Graph::VertexID> orig2copy_data(origin.numVertices());
//     typename Graph::IsoMap m(
//             make_iterator_property_map(orig2copy_data.begin(), get(boost::vertex_index, *(origin.graph)  )));
// 
//     // copy the graph and attach the mapping to it
//     boost::copy_graph(*(origin.graph), *(copy.graph), boost::orig_to_copy(m));
//     copy.mappingToStructureGraph = m;
//     return copy;
// }
// 
// 
// template<typename AttributeGraph1, typename AttributeGraph2>
// struct transform_vertex_copier
// {
//     using Graph1 = typename AttributeGraph1::BGLGraph;
//     using Graph2 = typename AttributeGraph2::BGLGraph;
// 
//     transform_vertex_copier(const Graph1& g1, Graph2& g2, const AttributeGraph2& ag2)
//       : vertex_all_map1(boost::get(boost::vertex_bundle, g1))
//       , vertex_all_map2(boost::get(boost::vertex_bundle, g2))
//       , ams2_ptr(ag2.attributeMapStore)
//     {}
//     template <typename Vertex1, typename Vertex2>
//     void operator()(const Vertex1& v1, Vertex2& v2) const {
//         //should be a pair <unsigned, Property<Attributes...>>
//         typename AttributeGraph1::VertexPropertyBundle v1prop = vertex_all_map1[v1];
// 
//         typename AttributeGraph2::VertexPropertyBundle v2prop;
//         v2prop.first = v1prop.first;
//         v2prop.second = typename AttributeGraph2::PropertyType(ams2_ptr);
//         v1prop.second.remapHandlesAndTags(v2prop.second);
// 
//         put(vertex_all_map2, v2, v2prop);
//     }
// 
//             typename boost::property_map<Graph1, boost::vertex_bundle_t>::const_type vertex_all_map1;
//     mutable typename boost::property_map<Graph2, boost::vertex_bundle_t>::type       vertex_all_map2;
//     std::shared_ptr<typename AttributeGraph2::StoreType> ams2_ptr;
// };
// 
// template<typename DestinationGraph, typename AttributeGraph1, typename AttributeGraph2>
// struct merge_vertex_copier
// {
//     using Destination = typename DestinationGraph::BGLGraph;
//     using Graph1 = typename AttributeGraph1::BGLGraph;
//     using Graph2 = typename AttributeGraph2::BGLGraph;
// 
//     merge_vertex_copier(Destination& d, const Graph1& g1, const Graph2& g2, const AttributeGraph2& ag2)
//       : vertex_all_map0(boost::get(boost::vertex_bundle, d))
//       , vertex_all_map1(boost::get(boost::vertex_bundle, g1))
//       , vertex_all_map2(boost::get(boost::vertex_bundle, g2))
//       , ams2_ptr(ag2.attributeMapStore)
//     {}
//     template <typename Vertex1, typename Vertex2>
//     void operator()(const Vertex1& v1, Vertex2& v2) const {
//         //should be a pair <unsigned, Property<Attributes...>>
//         typename AttributeGraph1::VertexPropertyBundle v1prop = vertex_all_map1[v1];
// 
//         typename AttributeGraph2::VertexPropertyBundle v2prop;
//         v2prop.first = v1prop.first;
//         v2prop.second = typename AttributeGraph2::PropertyType(ams2_ptr);
//         v1prop.second.remapHandlesAndTags(v2prop.second);
// 
//         put(vertex_all_map2, v2, v2prop);
//     }
// 
//     mutable typename boost::property_map<Destination, boost::vertex_bundle_t>::type  vertex_all_map0;
//             typename boost::property_map<Graph1, boost::vertex_bundle_t>::const_type vertex_all_map1;
//             typename boost::property_map<Graph2, boost::vertex_bundle_t>::const_type vertex_all_map2;
// 
//     std::shared_ptr<typename AttributeGraph2::StoreType> ams2_ptr;
// };
// 
// template<typename AttributeGraph1, typename AttributeGraph2>
// struct delete_vertex_copier
// {
//     using Graph1 = typename AttributeGraph1::BGLGraph;
//     using Graph2 = typename AttributeGraph2::BGLGraph;
// 
//     delete_vertex_copier(const Graph1& g1, Graph2& g2, const AttributeGraph2& ag2)
//       : vertex_all_map1(boost::get(boost::vertex_bundle, g1))
//       , vertex_all_map2(boost::get(boost::vertex_bundle, g2))
//       , ams2_ptr(ag2.attributeMapStore)
//     {}
//     template <typename Vertex1, typename Vertex2>
//     void operator()(const Vertex1& v1, Vertex2& v2) const {
//         //should be a pair <unsigned, Property<Attributes...>>
//         typename AttributeGraph1::VertexPropertyBundle v1prop = vertex_all_map1[v1];
// 
//         typename AttributeGraph2::VertexPropertyBundle v2prop;
//         v2prop.first = v1prop.first;
//         v2prop.second = typename AttributeGraph2::PropertyType(ams2_ptr);
// 
//         put(vertex_all_map2, v2, v2prop);
//     }
// 
//             typename boost::property_map<Graph1, boost::vertex_bundle_t>::const_type vertex_all_map1;
//     mutable typename boost::property_map<Graph2, boost::vertex_bundle_t>::type       vertex_all_map2;
//     std::shared_ptr<typename AttributeGraph2::StoreType> ams2_ptr;
// 
// };
// 
// template<typename AttributeGraph1, typename AttributeGraph2>
// struct transform_edge_copier
// {
//     using Graph1 = typename AttributeGraph1::BGLGraph;
//     using Graph2 = typename AttributeGraph2::BGLGraph;
// 
//     transform_edge_copier(const Graph1& g1, Graph2& g2, const AttributeGraph2& ag2)
//       : edge_all_map1(boost::get(boost::edge_bundle, g1))
//       , edge_all_map2(boost::get(boost::edge_bundle, g2))
//       , ams2_ptr(ag2.attributeMapStore)
//     {}
//     template <typename Edge1, typename Edge2>
//     void operator()(const Edge1& e1, Edge2& e2) const {
//         //should be a pair <unsigned, Property<Attributes...>>
//         typename AttributeGraph1::EdgePropertyBundle e1prop = edge_all_map1[e1];
//         typename AttributeGraph2::EdgePropertyBundle e2prop;
// 
//         e2prop.first = e1prop.first;
//         e2prop.second = typename AttributeGraph2::PropertyType(ams2_ptr);
//         e1prop.second.remapHandlesAndTags(e2prop.second);
// 
//         put(edge_all_map2, e2, e2prop);
//     }
// 
//             typename boost::property_map<Graph1, boost::edge_bundle_t>::const_type edge_all_map1;
//     mutable typename boost::property_map<Graph2, boost::edge_bundle_t>::type       edge_all_map2;
//     std::shared_ptr<typename AttributeGraph2::StoreType> ams2_ptr;
// 
// };
// 
// template<typename AttributeGraph1, typename AttributeGraph2>
// struct delete_edge_copier
// {
//     using Graph1 = typename AttributeGraph1::BGLGraph;
//     using Graph2 = typename AttributeGraph2::BGLGraph;
// 
//     delete_edge_copier(const Graph1& g1, Graph2& g2, const AttributeGraph2& ag2)
//       : edge_all_map1(boost::get(boost::edge_bundle, g1))
//       , edge_all_map2(boost::get(boost::edge_bundle, g2))
//       , ams2_ptr(ag2.attributeMapStore)
//     {}
//     template <typename Edge1, typename Edge2>
//     void operator()(const Edge1& e1, Edge2& e2) const {
//         //should be a pair <unsigned, Property<Attributes...>>
//         typename AttributeGraph1::EdgePropertyBundle e1prop = edge_all_map1[e1];
//         typename AttributeGraph2::EdgePropertyBundle e2prop;
// 
//         e2prop.first = e1prop.first;
//         e2prop.second = typename AttributeGraph2::PropertyType(ams2_ptr);
//         put(edge_all_map2, e2, e2prop);
//     }
// 
//             typename boost::property_map<Graph1, boost::edge_bundle_t>::const_type edge_all_map1;
//     mutable typename boost::property_map<Graph2, boost::edge_bundle_t>::type       edge_all_map2;
//     std::shared_ptr<typename AttributeGraph2::StoreType> ams2_ptr;
// };
// 
// template<typename GraphDestination, typename GraphOrigin>
// GraphDestination transformInto(const GraphOrigin& origin)
// {
//     GraphDestination copy;
// 
//     transform_vertex_copier <GraphOrigin, GraphDestination> vc(*(origin.graph), *(copy.graph), copy);
//     transform_edge_copier   <GraphOrigin, GraphDestination> ec(*(origin.graph), *(copy.graph), copy);
//     boost::copy_graph(*(origin.graph), *(copy.graph), boost::vertex_copy(vc).edge_copy(ec));
// 
//     // create a map to store the mapping (initializes with old values)
//     /* std::vector<typename GraphDestination::VertexID> orig2copy_data(origin.numVertices()); */
//     /* typename GraphOrigin::IsoMap m( */
//     /*         make_iterator_property_map(orig2copy_data.begin(), get(boost::vertex_index, *(origin.graph)  ))); */
//     /* boost::copy_graph(*(origin.graph), *(copy.graph), boost::vertex_copy(vc).edge_copy(ec).orig_to_copy(m)); */
//     /* copy.mappingToStructureGraph = m; */
//     return copy;
// }
// 
// template<typename GraphDestination, typename GraphOrigin>
// GraphDestination transformIntoEdgeOnly(const GraphOrigin& origin)
// {
//     GraphDestination copy;
// 
//     delete_vertex_copier  <GraphOrigin, GraphDestination> vc(*(origin.graph), *(copy.graph), copy);
//     transform_edge_copier <GraphOrigin, GraphDestination> ec(*(origin.graph), *(copy.graph), copy);
//     boost::copy_graph(*(origin.graph), *(copy.graph), boost::vertex_copy(vc).edge_copy(ec));
//     return copy;
// }
// 
// template<typename GraphDestination, typename GraphOrigin>
// GraphDestination transformIntoVertexOnly(const GraphOrigin& origin)
// {
//     GraphDestination copy;
// 
//     transform_vertex_copier <GraphOrigin, GraphDestination> vc(*(origin.graph), *(copy.graph), copy);
//     delete_edge_copier      <GraphOrigin, GraphDestination> ec(*(origin.graph), *(copy.graph), copy);
//     boost::copy_graph(*(origin.graph), *(copy.graph), boost::vertex_copy(vc).edge_copy(ec));
//     return copy;
// }
// 
// /**
//  * Copy nodes from one graph to another, then combine the structures.
//  *
//  * All nodes and vertices from subGraph are copied into parentGraph.
//  * After that, a directed edge from the parent's root node to the (copied)
//  * subGraph's root node is created.
//  * The function assumes that both graphs have identical types.
//  * The function also assumes, that the 'root' member variable is defined
//  * for both graphs.
//  *
//  * @param parentGraph the graph that the subGraph will be attached to
//  * @param subGraph the graph that will be attached
//  *
//  * @return the VertexID of the root node of the newly attached subGraph.
//  */
// template<typename... Attributes>
// typename AttributeGraph<Attributes...>::VertexID attachSubtree(
//     AttributeGraph<Attributes...>& parentGraph
//     , const AttributeGraph<Attributes...>& subGraph
//     )
// {
//     decltype(auto) dout = dout::Dout::getInstance();
//     dout(dout::Flags::DEBUG) << "in attachSubtree with same types" << std::endl;
//     using Graph = AttributeGraph<Attributes...>;
//     using VertexID = typename Graph::VertexID;
//     using Map = std::map<VertexID, VertexID>;
// 
//     Map m;
//     boost::associative_property_map<Map> mapV(m);
// 
//     transform_vertex_copier <Graph, Graph> vc(*(subGraph.graph), *(parentGraph.graph), parentGraph);
//     transform_edge_copier   <Graph, Graph> ec(*(subGraph.graph), *(parentGraph.graph), parentGraph);
// 
//     //parentGraph += subGraph
//     boost::copy_graph(
//         *(subGraph.graph),
//         *(parentGraph.graph),
//         boost::vertex_copy(vc).edge_copy(ec).orig_to_copy(mapV)
//     );
// 
//     VertexID subGraphRoot = mapV[subGraph.root];
// //    parentGraph.addEdge(parentGraph.root, subGraphRoot);
// 
//     dout(dout::Flags::DEBUG) << "in attachSubtree with same types end" << std::endl;
//     return subGraphRoot;
// 
// }
// 
// /**
//  * Wrapper function for attachSubtree that allows the graphs to have
//  * different types.
//  *
//  * The types of the subgraph have to be a subset of the types of the parent graph
//  */
// template<typename ParentGraph, typename SubGraph>
// typename ParentGraph::VertexID attachSubtree(
//     ParentGraph& parentGraph
//     , const SubGraph& subGraph
//     )
// {
//     decltype(auto) dout = dout::Dout::getInstance();
//     dout(dout::Flags::DEBUG) << "in attachSubtree to transform types" << std::endl;
//     static_assert(
//         hana::is_subset(
//             SubGraph::AttributesHanaSet,
//             ParentGraph::AttributesHanaSet
//         ),
//         "Subgraph is not a subgraph-type of Parent. Can not attach."
//     );
//     auto newSubGraph = transformInto<ParentGraph>(subGraph);
//     dout(dout::Flags::DEBUG) << "in attachSubtree to transform types end" << std::endl;
// 
//     return attachSubtree(parentGraph, newSubGraph) ;
// }
// 
// 
// 
// 
// 
// 
// } /* graph */
// } /* dodo */
