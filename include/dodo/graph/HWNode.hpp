#pragma once

#include <vector>

#include <dodo/graph/AttributeGraph.hpp>


namespace dodo
{
namespace graph
{

template<typename... AttributeTypes>
class AttributeGraph;

template<typename... AttributeTypes>
struct HWNode
{
    using AGraph = AttributeGraph<AttributeTypes...>;
    typename AGraph::VertexID id;
    AGraph* aGraph;

    HWNode(typename AGraph::VertexID pid, AGraph* pAGraph)
        :
        id(pid),
        aGraph(pAGraph)
    {}

    HWNode(){}


    typename AGraph::EdgeID consistsOf(HWNode other)
    {
        return aGraph->addEdge(id, other.id);
    }


    std::vector<typename AGraph::EdgeID> consistsOf(std::initializer_list<HWNode> otherlist)
    {
        std::vector<typename AGraph::EdgeID> edges;
        for(HWNode other : otherlist)
        {
            edges.push_back(aGraph->addEdge(id, other.id));
        }
        return edges;
    }

};

} /* graph */
} /* dodo */
