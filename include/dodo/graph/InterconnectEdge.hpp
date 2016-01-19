#pragma once

namespace dodo
{
namespace graph
{

template<typename T_Graph>
struct InterconnectEdge
{
    const typename T_Graph::EdgeID id;
    const T_Graph* graph;

    InterconnectEdge(
        const typename T_Graph::EdgeID eid,
        const T_Graph* pAGraph
    ) :
        id{eid},
        graph{pAGraph}
    {}

};

} /* graph */
} /* dodo */
