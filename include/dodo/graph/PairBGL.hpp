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
#include <memory>


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
template<
    class T_VertexProperty = SimpleProperty,
    class T_EdgeProperty = SimpleProperty
>
class PairBGL :
    public SimpleBGL<
        boost::property<boost::vertex_index_t, size_t, std::pair<unsigned, T_VertexProperty>>,
        boost::property<boost::edge_index_t, size_t, std::pair<unsigned, T_EdgeProperty>>,
        boost::multisetS,
        boost::listS,
        boost::undirectedS
    >
{

public:
    // Public typedefs
    typedef T_VertexProperty                                                VertexProperty;
    typedef T_EdgeProperty                                                  EdgeProperty;

    typedef std::pair<unsigned, VertexProperty>                             VertexPropertyBundle;
    typedef std::pair<unsigned, EdgeProperty>                               EdgePropertyBundle;

    using Graph = SimpleBGL<
        boost::property<boost::vertex_index_t, size_t, std::pair<unsigned, T_VertexProperty>>,
        boost::property<boost::edge_index_t, size_t, std::pair<unsigned, T_EdgeProperty>>,
        boost::multisetS,
        boost::listS,
        boost::undirectedS
    >;


    using BGLGraph = typename Graph::BGLGraph;

    using VertexID = typename Graph::VertexID;
    using EdgeID = typename Graph::EdgeID;
    using InEdgeIter = typename Graph::InEdgeIter;
    using OutEdgeIter = typename Graph::OutEdgeIter;
    using AdjacentVertexIter = typename Graph::AdjacentVertexIter;
    using AllVertexIter = typename Graph::AllVertexIter;
    using AllEdgeIter = typename Graph::AllEdgeIter;


    // Member
    std::vector<EdgeID> edgeIdMap;
    size_t edgeIDCounter=0;
    size_t vertexIDCounter=0;

public:

    void setVertexProperty(VertexID vertex, const VertexProperty& value){
        (*this->graph)[vertex].second = value;
    }

    void setVertexProperty(VertexID vertex, VertexPropertyBundle propPair){
        (*this->graph)[vertex] = propPair;
    }

    void setEdgeProperty(EdgeID edge, EdgeProperty value){
        EdgePropertyBundle propPair = (*this->graph)[edge];
        (*this->graph)[edge] = std::make_pair(propPair.first, value);
    }

    void setEdgeProperty(EdgeID edge, EdgePropertyBundle propPair){
        (*this->graph)[edge] = propPair;
    }

    /**
     * @brief Returns the property of *vertex*.
     *
     */
    VertexPropertyBundle& getVertexProperty(const VertexID vertex){
        return (*this->graph)[vertex];
    }

    /**
     * @brief Return the property of *edge*.
     *
     */
    EdgePropertyBundle& getEdgeProperty(const EdgeID edge){
        return (*this->graph)[edge];
    }

    EdgePropertyBundle& getEdgeProperty(const unsigned edge){
        return getEdgeProperty(edgeIdMap.at(edge));
    }

    VertexID addVertex(const VertexProperty& vp){
        auto v = addVertex();
        (*this->graph)[v].second = vp;
        return v;
    }

    VertexID addVertex(){
        auto v = boost::add_vertex(*this->graph);
        (*this->graph)[v].first = vertexIDCounter++;
        return v;
    }

    EdgeID addEdge(const VertexID srcVertex, const VertexID targetVertex)
    {
        auto e = boost::add_edge(srcVertex, targetVertex, (*this->graph)).first;
        (*this->graph)[e].first = edgeIDCounter++;
        return e;
    }

    EdgeID addEdge(const VertexID srcVertex, const VertexID targetVertex, const EdgeProperty& ep)
    {
        auto e = addEdge(srcVertex, targetVertex);
        (*this->graph)[e].second = ep;
        return e;
    }

};



} /* graph */
} /* dodo */

