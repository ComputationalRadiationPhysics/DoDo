#pragma once


#include <map>

#include <boost/graph/graphml.hpp>

#include "detail/SimpleBGLBase.hpp"
#include "detail/MemberIterator.hpp"


namespace dodo
{
    namespace graph
    {


/** Generic printer for SimpleBGL-Based Graphs with custom Properties
 *
 * When no dynamic properties are given, writes only the structural information
 * of the Graph.
 * Custom dynamic properties can be submitted to improve the readability of the
 * output.
 *
 *
 *
 *
 */
        template<
            typename T_Graph,
            typename T_SFINAE = typename std::enable_if<
                std::is_base_of<
                    detail::SimpleBGLBase, T_Graph
                >::value
            >::type
        >
        void simpleBGLWriter(
            std::ostream &os,
            const T_Graph &graph,
            boost::dynamic_properties dp = boost::dynamic_properties()
        )
        {
            using IndexMap = std::map<typename T_Graph::VertexID, std::size_t>;
            IndexMap im;
            boost::associative_property_map<IndexMap> propMapIndex(im);
            auto allV = graph.getVertices();
            int index = 0;

            // ensure monotone values for the node-IDs (necessary when using listS)
            for (auto i = allV.first; i != allV.second; ++i)
            {
                propMapIndex[*i] = index++;
            }

            boost::write_graphml(os, *(graph.graph), propMapIndex, dp);
        }


/**
 * Printer for BGL graph
 *
 * Prints the whole BGL graph including the necessary properties
 * for vertices and edges. The function writes to an output stream the
 * graph in the .graphml format.
 *
 * @param os    an output stream to write to
 * @param graph the BGL graph
 */
        template<
            typename T_Graph,
            typename T_SFINAE = typename std::enable_if<
                std::is_base_of<
                    detail::SimpleBGLBase, T_Graph
                >::value
            >::type
        >
        void defaultBGLWriter(
            std::ostream &os,
            const T_Graph &graph
        )
        {
            auto &g = *(graph.graph);
            boost::dynamic_properties dp;

            // extract all interesting properties
            detail::member_iterator<boost::vertex_bundle_t>(dp, g);
            detail::member_iterator<boost::edge_bundle_t>(dp, g);

            graph::simpleBGLWriter(os, graph, dp);
        }


/**
 * Generic Printer for SimpleBGL-Based Graphs
 *
 * Does only print the structural graph without any properties. The function writes
 * to an output stream the graph in the .graphml format.
 *
 * @param os    an output stream to write to
 * @param graph the BGL graph you want to print. Should inherit from SimpleBGLBase
 *              to be printable with this function
 */
        template<
            typename T_Graph,
            typename T_SFINAE = typename std::enable_if<
                std::is_base_of<
                    detail::SimpleBGLBase, T_Graph
                >::value
            >::type
        >
        std::ostream &operator<<(std::ostream &os, const T_Graph &graph)
        {
            defaultBGLWriter(os, graph);
            return os;
        }


/**
 * Adapter for operator<< to work with weak_ptrs of objects inheriting from SimpleBGLBase
 */
        template<
            typename T_Graph,
            typename T_SFINAE = typename std::enable_if<
                std::is_base_of<
                    detail::SimpleBGLBase, T_Graph
                >::value
            >::type
        >
        std::ostream &operator<<(std::ostream &os, const std::weak_ptr<T_Graph> graph)
        {
            return os << *(graph.lock());
        }


/**
 * Adapter for operator<< to work with shared_ptrs of objects inheriting from SimpleBGLBase
 */
        template<
            typename T_Graph,
            typename T_SFINAE = typename std::enable_if<
                std::is_base_of<
                    detail::SimpleBGLBase, T_Graph
                >::value
            >::type
        >
        std::ostream &operator<<(std::ostream &os, const std::shared_ptr<T_Graph> graph)
        {
            return os << *graph;
        }


    }
}
