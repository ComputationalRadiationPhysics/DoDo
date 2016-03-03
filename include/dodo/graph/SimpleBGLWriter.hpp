#pragma once

#include <map>
#include "detail/SimpleBGLBase.hpp"


namespace dodo
{
namespace graph
{


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
std::ostream& operator<<(std::ostream& os, const T_Graph& graph)
{
    using IndexMap = std::map<typename T_Graph::VertexID , std::size_t>;
    IndexMap im;
    boost::associative_property_map<IndexMap> propMapIndex(im);
    auto allV = graph.getVertices();
    int index=0;

    // ensure monotone values for the node-IDs (necessary when using listS)
    for(auto i=allV.first ; i!=allV.second ; ++i)
    {
        propMapIndex[*i] = index++;
    }

    boost::dynamic_properties dp;
    boost::write_graphml(os, *(graph.graph), propMapIndex, dp);
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
std::ostream& operator<<(std::ostream& os, const std::weak_ptr<T_Graph> graph)
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
std::ostream& operator<<(std::ostream& os, const std::shared_ptr<T_Graph> graph)
{
    return os << *graph;
}


}
}
