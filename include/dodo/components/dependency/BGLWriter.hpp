#pragma once


#include "BGL.hpp"



namespace dodo
{
namespace components
{
namespace dependency
{

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
std::ostream &operator<<(std::ostream &os, const BGL &graph)
{
    using IndexMap = std::map<BGL::VertexID, std::size_t>;
    IndexMap im;
    boost::associative_property_map<IndexMap> propMapIndex(im);
    auto allV = graph.getVertices();
    int index = 0;

    // ensure monotone values for the node-IDs (necessary when using listS)
    for (auto i = allV.first; i != allV.second; ++i)
    {
        propMapIndex[*i] = index++;
    }

    auto &g = *(graph.graph);
    boost::dynamic_properties dp;

    // extract all interesting properties
    dp.property("id", boost::get(&Vertex::id, g));
    dp.property("from_Port", boost::get(&Edge::from, g));
    dp.property("to_Port", boost::get(&Edge::to, g));

    write_graphml(os, g, propMapIndex, dp);
    return os;
}


}
}
}