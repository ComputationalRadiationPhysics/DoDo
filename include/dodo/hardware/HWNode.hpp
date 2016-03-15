#pragma once


#include <vector>

#include <dodo/graph/AttributeGraph.hpp>


namespace dodo
{
    namespace hardware
    {

        template<typename... AttributeTypes>
        class AttributeGraph;


        template<typename... AttributeTypes>
        struct HWNode
        {
            using AGraph = AttributeGraph<AttributeTypes...>;
            typename AGraph::VertexID id;
            AGraph *aGraph;


            HWNode(typename AGraph::VertexID pid, AGraph *pAGraph)
                :
                id(pid),
                aGraph(pAGraph)
            {}


        };

    } /* graph */
} /* dodo */
