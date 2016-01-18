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

        open.push_back(rootVertex);

        dout(dout::Flags::DEBUG) << "added Root Vertex" << std::endl;

        while( ! open.empty() )
        {
            VertexBase_t current = open.front();
            open.pop_front();

            dout(dout::Flags::DEBUG) << "retrieved vertex (" << current.id.get() << ")" << std::endl;

            open.insert(open.end(), current.children.begin(), current.children.end());

            if(Predicate::check(current))
            {
                closed.push_back(current);
            } else
            {
                to_delete.push_back(current);
            }
        }

        dout(dout::Flags::DEBUG) << "Processed consistsOf graph:" << std::endl;
        dout(dout::Flags::DEBUG) << "closed: " << closed.size() << std::endl;
        dout(dout::Flags::DEBUG) << "to_delete: " << to_delete.size() << std::endl;

        auto reducedIGraph = std::make_shared<Interconnect_t>();
        boost::copy_graph(*(interconnectGraph->graph), *(reducedIGraph->graph), boost::orig_to_copy(mapWrapper));

        for(VertexBase_t v : to_delete)
        {
            dout(dout::Flags::DEBUG) << "retrieved vertex (" << v.id.get() << ")" << std::endl;
            dout(dout::Flags::DEBUG) << "old iid: " << interconnectGraph->mapping[v.id] << ")" << std::endl;
            // id of interconnect node that corresponds to Vertex v in context of new interconnect graph
            auto iid = isoMap[interconnectGraph->mapping[v.id]];
            dout(dout::Flags::DEBUG) << "new iid: " << iid << ")" << std::endl;

            auto neighbors = reducedIGraph->getAdjacentVertices(iid);
            auto nNeighbors = std::distance(neighbors.first, neighbors.second);
            dout(dout::Flags::DEBUG) << "number Of Neighbors: " << nNeighbors << std::endl;

            if(nNeighbors == 2)
            // else, if iid means a linear node, collapse it
            {
                dout(dout::Flags::DEBUG) << "removal of linear topology" << std::endl;
                InterconnectID a = *(neighbors.first);
                InterconnectID b = *(++(neighbors.first));

                reducedIGraph->mergeLinearPaths(a, iid, b);
                reducedIGraph->mergeLinearPaths(b, iid, a);

            }else if(nNeighbors > 2)
            // else, if iid is center of a star topology, create fully connected mesh of adjacent vertices
            {
                dout(dout::Flags::DEBUG) << "removal of start topology not implemented " << std::endl;
            }
            reducedIGraph->removeVertex(iid);
        }

        return reducedIGraph;

    }



};


} /* graph */
} /* dodo */
