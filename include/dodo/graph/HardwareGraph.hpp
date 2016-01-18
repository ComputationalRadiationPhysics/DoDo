#pragma once

#include "HardwareGraphVertex.hpp"

namespace dodo
{
namespace graph
{

// template<
//     template <
//         typename T_ConsistsOfProperties,
//         typename T_InterconnectProperties
//     > typename T_RootVertex
// >

template<typename T_Vertex>
class HardwareGraph
{

    using ConsistsOfPropertes = typename T_Vertex::Properties;
    using InterconnectProperties = typename T_Vertex::InterconnectProperties;
    using VertexBase_t = typename T_Vertex::ConsistsOfStructure::value_type;

public:
    using Vertex = T_Vertex;
    using Interconnect_t = InterconnectGraph<InterconnectProperties>;

private:
    std::shared_ptr<Interconnect_t> interconnectGraph;
    Vertex rootVertex;

public:
    HardwareGraph() :
      interconnectGraph(std::make_shared<Interconnect_t>()),
      rootVertex(utility::TreeID(), interconnectGraph)
    {}

    template<typename Predicate>
    std::shared_ptr<Interconnect_t> getSubgraph()
    {
        auto& dout = dout::Dout::getInstance();
        std::list<VertexBase_t> open;
        std::list<VertexBase_t> closed;
        std::list<VertexBase_t> to_delete;

        using InterconnectID = typename Vertex::InterconnectID;
        using Map = std::map<InterconnectID, InterconnectID>;
        Map isoMap;
        boost::associative_property_map<Map> mapWrapper(isoMap);
        auto reducedIGraph = std::make_shared<Interconnect_t>();
        boost::copy_graph(*(interconnectGraph->graph), *(reducedIGraph->graph), boost::orig_to_copy(mapWrapper));

        open.push_back(rootVertex);

        while( ! open.empty() )
        {
            VertexBase_t current = open.front();
            open.pop_front();

            open.insert(open.end(), current.children.begin(), current.children.end());

            if(Predicate::check(current))
            {
                //closed.push_back(current);
                reducedIGraph->mapping[current.id] = isoMap[interconnectGraph->mapping[current.id]];
                reducedIGraph->stableMapping[current.id] = interconnectGraph->stableMapping[current.id];
            } else
            {
                to_delete.push_back(current);
            }
        }

        dout(dout::Flags::DEBUG) << "Processed consistsOf graph:" << std::endl;
        dout(dout::Flags::DEBUG) << "closed: " << closed.size() << std::endl;
        dout(dout::Flags::DEBUG) << "to_delete: " << to_delete.size() << std::endl;


        for(VertexBase_t v : to_delete)
        {
            // id of interconnect node that corresponds to Vertex v in context of new interconnect graph
            auto iid = isoMap[interconnectGraph->mapping[v.id]];
            reducedIGraph->mergeStarTopology(iid);
        }
        return reducedIGraph;

    }



};


} /* graph */
} /* dodo */
