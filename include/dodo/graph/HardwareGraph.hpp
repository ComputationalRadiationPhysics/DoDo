#pragma once

#include "HardwareGraphVertex.hpp"
#include "InterconnectGraph.hpp"
#include "../utility/tree_id.hpp"

namespace dodo
{
namespace graph
{

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
      interconnectGraph{ std::make_shared<Interconnect_t>() },
      rootVertex{utility::TreeID(), interconnectGraph}
    {}

    template<typename Predicate>
    std::shared_ptr<Interconnect_t>
    getSubgraph(const Predicate& predicate)
    {
        dout::Dout& dout{ dout::Dout::getInstance() };
        using InterconnectID = typename Vertex::InterconnectID;
        using IsoMap = std::map<InterconnectID, InterconnectID>;
        using VertexList = std::list<VertexBase_t>;

        VertexList openVertices;
        VertexList to_delete;

        IsoMap isoMap;
        boost::associative_property_map<IsoMap> mapWrapper{isoMap};
        auto reducedIGraph( std::make_shared<Interconnect_t>() );

        boost::copy_graph(*(interconnectGraph->graph), *(reducedIGraph->graph), boost::orig_to_copy(mapWrapper));

        openVertices.push_back(rootVertex);

        while( ! openVertices.empty() )
        {
            VertexBase_t current{ openVertices.front() };
            openVertices.pop_front();

            openVertices.insert(openVertices.end(), current.children.begin(), current.children.end());

            if( predicate(current) )
            {
                const auto pos( current.id );
                reducedIGraph->mapping[pos] = isoMap[interconnectGraph->mapping[pos]];
                reducedIGraph->stableMapping[pos] = interconnectGraph->stableMapping[pos];
            } else
            {
                to_delete.push_back(current);
            }
        }

        dout(dout::Flags::DEBUG) << "Processed consistsOf graph:" << std::endl;
        dout(dout::Flags::DEBUG) << "to_delete: " << to_delete.size() << std::endl;

        for(const VertexBase_t& v : to_delete)
        {
            // id of interconnect node that corresponds to Vertex v in context of new interconnect graph
            const auto iid( isoMap[interconnectGraph->mapping[v.id]] );
            reducedIGraph->mergeStarTopology(iid);
        }
        return reducedIGraph;
    }

};

} /* graph */
} /* dodo */
