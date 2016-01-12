#pragma once

#include <vector>

#include <dodo/graph/InterconnectGraph.hpp>


namespace dodo
{
namespace graph
{


template<typename T_Graph>
struct InterconnectEdge
{
    typename T_Graph::EdgeID id;
    T_Graph* graph;

    InterconnectEdge(typename T_Graph::EdgeID eid, T_Graph* pAGraph)
        :
        id(eid),
        graph(pAGraph)
    {}




};

} /* graph */
} /* dodo */
