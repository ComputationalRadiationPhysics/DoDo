#pragma once
#include <vector>
#include <iostream>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/copy.hpp>
#include <tuple>
#include <string>
#include <fstream> /* std::fstream */
#include <utility> /* std::pair, std::make_pair */
#include <stdexcept>


namespace dodo{
namespace graph{

struct SimpleProperty{
    typedef unsigned ID;
    SimpleProperty() : id(0){}
    SimpleProperty(ID id) : id(id){}

    ID id;
};

/**************************************************************************
 * @class BGL
 *
 * @brief A class to describe directed graphs.
 *
 * GraphPolicy on basis of the boost graph library.
 *
 ***************************************************************************/
template <class T_VertexProperty = SimpleProperty, class T_EdgeProperty = SimpleProperty>
class BGL {

public:
    // Public typedefs
    typedef T_VertexProperty                                                VertexProperty;
    typedef T_EdgeProperty                                                  EdgeProperty;

    typedef std::pair<unsigned, VertexProperty>                             VertexPropertyBundle;
    typedef std::pair<unsigned, EdgeProperty>                               EdgePropertyBundle;


    // BGL typdefs
    typedef boost::adjacency_list<boost::multisetS,
            boost::listS,
            boost::bidirectionalS,
            // VertexPropertyBundle,
            // EdgePropertyBundle
            boost::property<boost::vertex_index_t, size_t, VertexPropertyBundle >,
            boost::property<boost::edge_index_t, size_t, EdgePropertyBundle >
    > BGLGraph;

    typedef typename BGLGraph::vertex_descriptor VertexID;
    typedef typename BGLGraph::edge_descriptor   EdgeID;

    typedef typename boost::graph_traits<BGLGraph>::in_edge_iterator   InEdgeIter;
    typedef typename boost::graph_traits<BGLGraph>::out_edge_iterator  OutEdgeIter;
    typedef typename boost::graph_traits<BGLGraph>::adjacency_iterator AdjacentVertexIter;
    typedef typename boost::graph_traits<BGLGraph>::vertex_iterator    AllVertexIter;
    typedef typename boost::graph_traits<BGLGraph>::edge_iterator      AllEdgeIter;



    // Member
    std::shared_ptr<BGLGraph> graph;

    std::vector<EdgeID> edgeIdMap;
    size_t edgeIDCounter=0;
    size_t vertexIDCounter=0;

public:


    BGL()
    {
        graph = std::make_shared<BGLGraph>();
    }


    /**
     * @brief Returns all vertices of the graph
     *
     */
    std::pair<AllVertexIter, AllVertexIter> getVertices(){
        return boost::vertices((*graph));
    }

    std::pair<AllEdgeIter, AllEdgeIter> getEdges(){
        return boost::edges((*graph));
    }

    /**
     * @brief Returns the edge between source and target vertex.
     *
     */
    std::pair<EdgeID, bool> getEdge(const VertexID source, const VertexID target){
        return boost::edge(source, target, *graph);
    }

    /**
     * @brief Returns all vertices, that are adjacent (connected) to *vertex*
     *
     */
    std::pair<AdjacentVertexIter, AdjacentVertexIter>  getAdjacentVertices(const VertexID id){
        return boost::adjacent_vertices(id, *graph);
    }

    /**
     * @brief Returns all outgoing edges of *srcVertex* paired with its target vertex.
     *
     */
    std::pair<OutEdgeIter, OutEdgeIter> getOutEdges(const VertexID id){
        return boost::out_edges(id, (*graph));
    }

    /**
     * @brief Returns all incoming edges to *targetVertex* paired with its source vertex.
     *
     */
    std::pair<InEdgeIter, InEdgeIter> getInEdges(const VertexID id) const {
        return boost::in_edges(id, (*graph));
    }

    /**
     * @brief Returns the local id of *vertex* in this graph.
     *
     * If this graph has no supergraph (hasSuperGraph()==false) then local ids are the same as global ids.
     */
    VertexID getLocalID(VertexProperty vertex){
        return (*graph).global_to_local(vertex.id);
    }

    void setVertexProperty(VertexID vertex, const VertexProperty& value){
        (*graph)[vertex].second = value;
    }

    void setVertexProperty(VertexID vertex, std::pair<unsigned, VertexProperty> propPair){
        (*graph)[vertex] = propPair;
    }

    void setEdgeProperty(EdgeID edge, EdgeProperty value){
        std::pair<unsigned, EdgeProperty> propPair = (*graph)[edge];
        (*graph)[edge] = std::make_pair(propPair.first, value);
    }

    void setEdgeProperty(EdgeID edge, std::pair<unsigned, EdgeProperty> propPair){
        (*graph)[edge] = propPair;
    }

    /**
     * @brief Returns the property of *vertex*.
     *
     */
    std::pair<unsigned, VertexProperty>& getVertexProperty(const VertexID vertex){
        return (*graph)[vertex];
    }

    /**
     * @brief Return the property of *edge*.
     *
     */
    std::pair<unsigned, EdgeProperty>& getEdgeProperty(const EdgeID edge){
        return (*graph)[edge];
    }

    std::pair<unsigned, EdgeProperty>& getEdgeProperty(const unsigned edge){
        return getEdgeProperty(edgeIdMap.at(edge));
    }

    /**
     * @brief Return the vertex to which *edge* points to.
     *
     */
    VertexID getEdgeTarget(const EdgeID edge){
        return boost::target(edge, (*graph));
    }

    VertexID getEdgeTarget(const unsigned edge){
        return getEdgeTarget(edgeIdMap.at(edge));
    }

    /**
     * @brief Return the vertex to which *edge* points from.
     *
     */
    VertexID getEdgeSource(const EdgeID edge){
        return boost::source(edge, (*graph));
    }

    VertexID getEdgeSource(const unsigned edge){
        return getEdgeSource(edgeIdMap.at(edge));
    }

    VertexID addVertex(const VertexProperty& vp){
        auto v = addVertex();
        (*graph)[v].second = vp;
        return v;
    }

    VertexID addVertex(){
        auto v = boost::add_vertex(*graph);
        (*graph)[v].first = vertexIDCounter++;
        return v;
    }



    // // TODO: make polymorphic over input GraphType,
    // //       then make sure that input GraphType's Attributes are a subset of
    // //       this graph's attributes
    // void addGraph(const BGLGraph& inputGraph, VertexID newAnchor){
    //     VertexID inputRoot = inputGraph.findRootVertexRecurse();
    //     attachSubtree(graph, newAnchor, inputGraph, inputRoot);
    // }

    // bool hasInEdges(VertexID v){
    //     auto inEdgeIterators = getInEdges(v);
    //     return inEdgeIterators.first != inEdgeIterators.second;
    // }

    // VertexID findRootVertexRecurse()
    // {
    //     VertexID v = *(getVertices().first);
    //     auto inEdgeIterators = getInEdges(v);

    //     while(inEdgeIterators.first != inEdgeIterators.second)
    //     {
    //        v = getEdgeSource(*(inEdgeIterators.first));
    //        inEdgeIterators = getInEdges(v);
    //     }
    //     return v;
    // }

    // VertexID findRootVertexIter()
    // {
    //     auto vIterators = getVertices();
    //     while(vIterators.first != vIterators.second)
    //     {
    //         if(hasInEdges(*(vIterators.first)))
    //         {
    //             ++(vIterators.first);
    //         } else
    //         {
    //             return *(vIterators.first);
    //         }
    //     }
    //     throw std::logic_error("Graph does not have an explicit root Node");
    // }

    EdgeID addEdge(const VertexID srcVertex, const VertexID targetVertex)
    {
        auto e = boost::add_edge(srcVertex, targetVertex, (*graph)).first;
        (*graph)[e].first = edgeIDCounter++;
        return e;
    }

    EdgeID addEdge(const VertexID srcVertex, const VertexID targetVertex, const EdgeProperty& ep)
    {
        auto e = addEdge(srcVertex, targetVertex);
        (*graph)[e].second = ep;
        return e;
    }

    void removeEdge(EdgeID e){
        boost::remove_edge(e, *graph);
    }

    size_t numVertices() const {
        return num_vertices(*graph);
    }

    void removeVertex(VertexID v){
        boost::remove_vertex(v, *graph);
    }

    std::pair<OutEdgeIter, OutEdgeIter> edgeRange(VertexID srcVertex, VertexID destVertex)
    {
        return boost::edge_range(srcVertex, destVertex, *graph);
    }








};



// template<typename Graph>
// void attachSubtree(
//         Graph& g1
//         , typename Graph::VertexID anchor
//         , const Graph& g2
//         , typename Graph::VertexID rootOfSubgraph
//         )
// {
//     using vertexID = typename Graph::VertexID;

//     // TODO: check how to copy properties in the best way. maybe replace clone
//     // member function with copy constructor/assignment

//     using index_map_t = typename boost::property_map<Graph, boost::vertex_index_t>::type;
//     using IsoMap = boost::iterator_property_map<typename std::vector<vertexID>::iterator, index_map_t, vertexID, vertexID&>;
//     std::vector<vertexID> orig2copy_data(num_vertices(g2));
//     IsoMap mapV = make_iterator_property_map(orig2copy_data.begin(), get(boost::vertex_index, g2));
//     boost::copy_graph( g2, g1, boost::orig_to_copy(mapV)  ); //means g1 += g2
//     vertexID newRoot = mapV[rootOfSubgraph];
//     return g1.addEdge(anchor, newRoot);
// }


} /* graph */
} /* dodo */

