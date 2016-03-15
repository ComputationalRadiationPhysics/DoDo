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

#include "detail/SimpleBGLBase.hpp"


namespace dodo
{
    namespace graph
    {

        template<
            typename T_VertexProperty = boost::no_property,
            typename T_EdgeProperty = boost::no_property,
            typename T_OutEdgeList = boost::vecS,
            typename T_VertexList = boost::vecS,
            typename T_Directed = boost::directedS
        >
        class SimpleBGL :
            detail::SimpleBGLBase
        {

        public:
            // Public typedefs
            typedef T_VertexProperty VertexProperty;
            typedef T_EdgeProperty EdgeProperty;

            // BGL typdefs
            typedef boost::adjacency_list<
                T_OutEdgeList,
                T_VertexList,
                T_Directed,
                T_VertexProperty,
                T_EdgeProperty
            > BGLGraph;

            typedef typename BGLGraph::vertex_descriptor VertexID;
            typedef typename BGLGraph::edge_descriptor EdgeID;

            typedef typename boost::graph_traits<BGLGraph>::in_edge_iterator InEdgeIter;
            typedef typename boost::graph_traits<BGLGraph>::out_edge_iterator OutEdgeIter;
            typedef typename boost::graph_traits<BGLGraph>::adjacency_iterator AdjacentVertexIter;
            typedef typename boost::graph_traits<BGLGraph>::vertex_iterator AllVertexIter;
            typedef typename boost::graph_traits<BGLGraph>::edge_iterator AllEdgeIter;

            // Member
            std::shared_ptr<BGLGraph> graph;
        public:


            SimpleBGL()
            {
                graph = std::make_shared<BGLGraph>();
            }


            /**
             * @brief Returns all vertices of the graph
             *
             */
            std::pair<AllVertexIter, AllVertexIter> getVertices() const
            {
                return boost::vertices((*graph));
            }


            std::pair<AllEdgeIter, AllEdgeIter> getEdges()
            {
                return boost::edges((*graph));
            }


            /**
             * @brief Returns the edge between source and target vertex.
             *
             */
            std::pair<EdgeID, bool> getEdge(const VertexID source, const VertexID target)
            {
                return boost::edge(source, target, *graph);
            }


            /**
             * @brief Returns all vertices, that are adjacent (connected) to *vertex*
             *
             */
            std::pair<AdjacentVertexIter, AdjacentVertexIter> getAdjacentVertices(const VertexID id)
            {
                return boost::adjacent_vertices(id, *graph);
            }


            /**
             * @brief Returns all outgoing edges of *srcVertex* paired with its target vertex.
             *
             */
            std::pair<OutEdgeIter, OutEdgeIter> getOutEdges(const VertexID id)
            {
                return boost::out_edges(id, (*graph));
            }


            /**
             * @brief Returns all incoming edges to *targetVertex* paired with its source vertex.
             *
             */
            std::pair<InEdgeIter, InEdgeIter> getInEdges(const VertexID id) const
            {
                return boost::in_edges(id, (*graph));
            }


            void setVertexProperty(VertexID vertex, const VertexProperty &value)
            {
                (*graph)[vertex] = value;
            }


            void setEdgeProperty(EdgeID edge, EdgeProperty &value)
            {
                (*graph)[edge] = value;
            }


            /**
             * @brief Returns the property of *vertex*.
             *
             */
            VertexProperty &getVertexProperty(const VertexID vertex)
            {
                return (*graph)[vertex];
            }


            /**
             * @brief Return the property of *edge*.
             *
             */
            EdgeProperty &getEdgeProperty(const EdgeID edge)
            {
                return (*graph)[edge];
            }


            /**
             * @brief Return the vertex to which *edge* points to.
             *
             */
            VertexID getEdgeTarget(const EdgeID edge)
            {
                return boost::target(edge, (*graph));
            }


            /**
             * @brief Return the vertex to which *edge* points from.
             *
             */
            VertexID getEdgeSource(const EdgeID edge)
            {
                return boost::source(edge, (*graph));
            }


            VertexID addVertex(const VertexProperty &vp)
            {
                auto v = addVertex();
                (*graph)[v] = vp;
                return v;
            }


            VertexID addVertex()
            {
                auto v = boost::add_vertex(*graph);
                return v;
            }


            EdgeID addEdge(const VertexID srcVertex, const VertexID targetVertex)
            {
                return boost::add_edge(srcVertex, targetVertex, (*graph)).first;
            }


            EdgeID addEdge(
                const VertexID srcVertex,
                const VertexID targetVertex,
                const EdgeProperty &ep
            )
            {
                auto e = addEdge(srcVertex, targetVertex);
                (*graph)[e] = ep;
                return e;
            }


            void removeEdge(EdgeID e)
            {
                boost::remove_edge(e, *graph);
            }


            size_t numVertices() const
            {
                return num_vertices(*graph);
            }


            void removeVertex(VertexID v)
            {
                boost::remove_vertex(v, *graph);
            }


            std::pair<OutEdgeIter, OutEdgeIter> edgeRange(VertexID srcVertex, VertexID destVertex)
            {
                return boost::edge_range(srcVertex, destVertex, *graph);
            }


        };

    }
}
